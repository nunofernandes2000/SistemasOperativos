#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "restaurant.h"
#include "notification.h"
#include "semaphore.h"
#include <sys/shm.h> //shared memory
#include <sys/wait.h> // for wait

#define exit_on_error(s, m) if (s < 0) { perror(m); exit(1); }
#define NUM_COOKS 3


void cook_process() {
    int msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "msgget");
    int shared_memory_id = shmget(1001, sizeof(SharedDish) * NUMBER_OF_DISHES, 0600 | IPC_CREAT);
    exit_on_error(shared_memory_id, "shmget");
    SharedDish* dish_status = (SharedDish*)shmat(shared_memory_id, NULL, 0);
    if (dish_status == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    int pid = getpid();
    srand(time(NULL) ^ (pid << 16));

    int mutex_id = create_mutex(0); // Obter o identificador do mutex
    int pedidos_espera_id = create_semaphore(PEDIDOS_ESPERA); // Obter o identificador do semáforo de pedidos

    printf("Cook %d started\n", pid); // Adicionando mensagem de depuração

    while (1) {
        printf("Cook %d waiting for a request\n", pid); // Adicionando mensagem de depuração
        down(pedidos_espera_id);
        // Lock the shared memory
        down(mutex_id); // lock mutex
        int dish = -1;
        int client_id = -1;
        for (int i = 0; i < NUMBER_OF_DISHES; i++) {
            if (dish_status[i].client_id != 0) {
                dish = dish_status[i].dish;
                client_id = dish_status[i].client_id;
                break;
            }
        }
        up(mutex_id); // unlock mutex

        if (dish == -1) {
            printf("Cook %d found no dish to prepare\n", pid);
            continue;
        }

        printf("Cook %d is preparing dish %s for client %d\n", pid, dishes[dish].name, client_id);
        send_notification(msg_id, client_id, PREPARATION);
        sleep(dishes[dish].preparation_time);
        send_notification(msg_id, client_id, READY);

        // Update the shared memory
        down(mutex_id); // lock mutex
        for (int i = 0; i < NUMBER_OF_DISHES; i++) {
            if (dish_status[i].client_id == client_id) {
                dish_status[i].client_id = 0;
                dish_status[i].dish = -1;
                break;
            }
        }
        up(mutex_id); // unlock mutex

        printf("Cook %d finished preparing dish %s for client %d\n", pid, dishes[dish].name, client_id);
    }
    shmdt(dish_status);
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < NUM_COOKS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Processo filho
            cook_process();
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Processo pai espera que todos os filhos terminem
    for (int i = 0; i < NUM_COOKS; i++) {
        wait(NULL);
    }

    return 0;
}
