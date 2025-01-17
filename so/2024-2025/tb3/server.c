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
    int status;
    REQUEST request;

    // connect to the message queue
    msg_id = msgget(1000, 0600 | IPC_CREAT);
    exit_on_error(msg_id, "Creation/Connection");
    //criar memoria partilhada
    //criar mutexes
    //criar semaforos

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
            send_notification(msg_id, request.pid, REQUESTED); // avisa o cliente que o pedido chegou-la

            //escrever(procurar um que estteja livre(status = -1000 por exemplo)) e fazer up
            //atualizar o status 


            // o codigo tem que ser alterado
            sleep(dishes[request.dish].preparation_time);

            printf("Request of client %d READY: %d - %s\n", request.pid, request.dish, dishes[request.dish].name);
            send_notification(msg_id, request.pid, READY);
        }
    }
}