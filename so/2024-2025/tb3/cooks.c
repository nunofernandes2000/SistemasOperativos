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
#include <pthread.h>
#include <sys/shm.h> //memoria compartilhada memoria

#define exit_on_error(s, m) if (s < 0) { perror(m); exit(1); }
#define NUM_COOKS 2

void* cook_thread(void* arg){
    int msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "msgget");
    int shared_memory_id = shmget(1001, sizeof(int) * NUMBER_OF_DISHES, 0600 | IPC_CREAT);
    int* dish_status = shmat(shared_memory_id, NULL, 0);
    int pid = getpid();
    srand(time(NULL) ^ (pid << 16));

    while(1) {
        down(PEDIDOS_ESPERA);
        // Lock the shared memory
        down(0); // lock mutex
        int dish = -1;
        for (int i = 0; i < NUMBER_OF_DISHES; i++) {
            if (dish_status[i] == 0) {
                dish_status[i] = pid;
                dish = i;
                break;
            }
        }
        up(0); // unlock mutex

        if (dish == -1) {
            continue;
        }

        printf("Cook %d is preparing dish %s\n", pid, dishes[dish].name);
        send_notification(msg_id, pid, PREPARATION);
        sleep(dishes[dish].preparation_time);
        send_notification(msg_id, pid, READY);

        // Update the shared memory
        down(0); // lock mutex
        dish_status[dish] = 0;
        up(0); // unlock mutex
    }
    shmdt(dish_status);
    return 0;
}

int main(int argc, char *argv[]) {
    pthread_t cooks[NUM_COOKS];

    for(int i = 0; i < NUM_COOKS; i++){
        pthread_create(&cooks[i], NULL, cook_thread, NULL);
    }

    for(int i = 0; i < NUM_COOKS; i++){
        pthread_join(cooks[i], NULL);
    }

    return 0;
}
