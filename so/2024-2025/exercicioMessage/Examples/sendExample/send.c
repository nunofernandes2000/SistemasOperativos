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
    msg_id = msgget(1000, 0600 | IPC_CREAT); // 1º parameter is the key, 2º is a flag to create the message queue if it doesn't exist
    /**
     * Exits the program if there is an error with the message queue creation or connection.
     *
     * @param msg_id The message queue identifier.
     * @param " Creation/Connection" The error message to display if there is an error.
     */
    exit_on_error(msg_id, " Creation/Connection");  
 
    // send a message 
    msg.type = 1; // message to type 1
    strcpy(msg.text, "Any message..."); 
    // 1º parameter is the message queue identifier
    // 2º is the message to send
    // 3º is the size of the message(descontando o tamnaho do type que é 4) - pode ser escrito da seguinte forma: sizeof(msg) - sizeof(msg.type)
    // 4º is the flag
    status = msgsnd(msg_id, &msg, sizeof(msg.text), 0);
    exit_on_error(status, "Send"); 
    printf("Message sent!\n"); 
} 
