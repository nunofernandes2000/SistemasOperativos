#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "restaurant.h"
#include "notification.h"

#define exit_on_error(s, m) if (s < 0) { perror(m); exit(1); }

int msg_id;

int generate_random(int min_value, int max_value) {
    return min_value + (rand() % (max_value - min_value + 1));
}

void client() {
    REQUEST request;
    NOTIFICATION_STATUS notification;
    int status;

    request.type = 1;
    request.pid = getpid();
    request.dish = generate_random(0, NUMBER_OF_DISHES - 1);

    printf("New client %d request %d - %s\n", getpid(), request.dish, dishes[request.dish].name);

    status = msgsnd(msg_id, &request, sizeof(request) - sizeof(long), 0);
    exit_on_error(status, "Send");

    while (1) {
        status = msgrcv(msg_id, &notification, sizeof(notification) - sizeof(long), getpid(), 0);
        exit_on_error(status, "Answer");

        printf("[%05d] Status changed: %d\n", getpid(), notification.status); // envia notificação do status do pedido
        

        if (notification.status == READY || notification.status == INVALID_DISH)
            return;
    }
}

int main( int argc, char *argv[]) {
    srand(time(NULL));

    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "Creation/Connection");

    while(1) {
        if(fork() == 0) {
            // Processo filho
            client();
            exit(0);
        } else {
            sleep(generate_random(3, 15));
        }
    }
}