#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
 
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); } 
 
typedef struct { 
   long type; 
   char text[250]; 
} msg_struct; 
 
int main(int argc, char *argv[])  
{ 
   int msg_id; 
   int status; 
   msg_struct msg; 
 
    // connect to the message queue 
    msg_id = msgget(1000, 0600 | IPC_CREAT); 
    exit_on_error(msg_id, "Creation/Connection"); 
 
    // receive a message (blocks if none) 
    status = msgrcv(msg_id, &msg, sizeof(msg.text), 1, IPC_NOWAIT); //o 0 é para bloquear a mensagem até que apareça uma mensagem daquele tipo/agora nao bloqueia por causa do IPC_NOWAIT
    exit_on_error(status, "Reception"); 
    printf("MESSAGE <%s>\n", msg.text); 
}