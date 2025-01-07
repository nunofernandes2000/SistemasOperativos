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
    return min_value + (rand() % (max_value - min_value));
}

void client() {
    REQUEST request;
    NOTIFICATION_STATUS notification;
    int status;

    request.type = 1;
    request.pid = getpid();
    request.dish = generate_random(0, NUMBER_OF_DISHES - 1);

    printf("New client %d request %d - %s\n", getpid(), dishes[request.dish].name);

    /* send request to server */
    status = msgsnd(msg_id, &request, sizeof(request) - sizeof(long), 0);
    exit_on_error(status, "Send");

    /* wait until the request is ready */
    while (1) {
        /* receive notification from server or cooks */
        status = msgrcv(msg_id, &notification, sizeof(notification) - sizeof(long), getpid(), 0); // 0 -> bloqueante
        exit_on_error(status, "Answer");

        printf("[%05d] Status changed: %d\n", getpid(), notification.status);

        if (notification.status == READY || notification.status == INVALID_DISH)
            return;
    }
}

int main( int argc, char *argv[]) {
    srand(time(NULL));

    // connect with the message queue
    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "Creation/Connection");

    while(1) {
        /* create a new client */
        if(fork())
            client();

        else
            /* wait for the next client */
            sleep(generate_random(3, 15));
    }
}