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
#include <sys/shm.h>

#define exit_on_error(s, m) if (s < 0) { perror(m); exit(1); }

int main(int argc, char *argv[]) {
    int msg_id;
    int status;
    REQUEST request;

    // connect to the message queue
    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "Creation/Connection");

    // create shared memory
    int shared_memory_id = shmget(1001, sizeof(int) * NUMBER_OF_DISHES, 0600 | IPC_CREAT);
    int* dish_status = shmat(shared_memory_id, NULL, 0);
    for(int i = 0; i < NUMBER_OF_DISHES; i++) {
        dish_status[i] = 0; // set all dishes as available
    }
    shmdt(dish_status); // detach the shared memory segment

    // create mutexes
    create_mutex(0);
    // create semaphores
    create_semaphore(PEDIDOS_ESPERA);

    printf("Server ready\n");

    while(1) {
        // receive requests from clients
        status = msgrcv(msg_id, &request, sizeof(request) - sizeof(long), 1, 0);
        exit_on_error(status, "Reception");
        if(request.dish < 0 || request.dish >= NUMBER_OF_DISHES) {
            send_notification(msg_id, request.pid , INVALID_DISH);
        }
        else {
            printf("Request received from client %d: %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            send_notification(msg_id, request.pid, REQUESTED); // notify the client that the request has been received

            down(0); // lock mutex
            dish_status = shmat(shared_memory_id, NULL, 0);
            for (int i = 0; i < NUMBER_OF_DISHES; i++) {
                if (dish_status[i] == 0) {
                    dish_status[i] = request.pid;
                    break;
                }
            }
            shmdt(dish_status);
            up(0); // unlock mutex

            up(PEDIDOS_ESPERA); // release a request to be prepared
            // the code needs to be changed
            sleep(dishes[request.dish].preparation_time);

            printf("Request of client %d READY: %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            send_notification(msg_id, request.pid, READY);
        }
    }
}