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

int main(int argc, char *argv[]) {
    int msg_id;
    int pid = getpid();
    int dish;
    REQUEST request;
    NOTIFICATION_STATUS notification;
    srand(time(NULL));

    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "msgget");

    while (1) {

        //down(pratos em espera)
        dish = rand() % NUMBER_OF_DISHES;
        request.type = 1;
        request.pid = pid;
        request.dish = dish;

        printf("Cook %d is preparing dish %s\n", pid, dishes[dish].name);
        send_notification(msg_id, pid, PREPARATION);
        sleep(dishes[dish].preparation_time);
        send_notification(msg_id, pid, READY);
    }

    return 0;
}
