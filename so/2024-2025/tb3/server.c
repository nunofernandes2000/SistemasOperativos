#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "restaurant.h"
#include "notification.h"
#include "semaphore.h"

#define exit_on_error(s, m) if (s < 0) { perror(m); exit(1); }

int main(int argc, char *argv[]) {
    int msg_id;
    int status;
    REQUEST request;

    // Conecta-se a fila de mesagens
    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "Creation/Connection");

    // Cria a memoria partilhada
    int shared_memory_id = shmget(1001, sizeof(SharedDish) * NUMBER_OF_DISHES, 0600 | IPC_CREAT);
    SharedDish* dish_status = (SharedDish*)shmat(shared_memory_id, NULL, 0);
    for (int i = 0; i < NUMBER_OF_DISHES; i++) {
        dish_status[i].client_id = 0; // Nenhum cliente
        dish_status[i].dish = -1;    // Nenhum prato
    }
    shmdt(dish_status);

    // Cria o mutex
    int mutex_id = create_mutex(0);
    // Cria o semaphore
    int pedidos_espera_id = create_semaphore(PEDIDOS_ESPERA);

    printf("Server ready\n");

    while (1) {
        // Reecebe os pedidos dos clientes
        status = msgrcv(msg_id, &request, sizeof(request) - sizeof(long), 1, 0);
        exit_on_error(status, "Reception");
        if (request.dish < 0 || request.dish >= NUMBER_OF_DISHES) {
            send_notification(msg_id, request.pid, INVALID_DISH);
        } else {
            printf("Request received from client %d: %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            send_notification(msg_id, request.pid, REQUESTED); // notifica o cliente que o pedido foi recebido

            down(mutex_id); // lock mutex
            dish_status = (SharedDish*)shmat(shared_memory_id, NULL, 0);
            int assigned = 0; //pedido  não atribuido
            for (int i = 0; i < NUMBER_OF_DISHES; i++) {
                if (dish_status[i].client_id == 0) {
                    dish_status[i].client_id = request.pid;
                    dish_status[i].dish = request.dish;
                    assigned = 1; //pedido atribuido
                    break;
                }
            }
            shmdt(dish_status);
            up(mutex_id); // unlock mutex

            // Se o pedido foi atribuido entra aqui
            if (assigned) {
                up(pedidos_espera_id); // release a request to be prepared
                printf("Request of client %d assigned: %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            } else {
                printf("No available slot for client %d request %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            }
        }
    }
}


