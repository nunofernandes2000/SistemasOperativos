#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
 
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); } 
 
typedef struct { 
    long type; 
    struct _msg_struct { 
        int pid; 
        char text[250]; 
    } msg; 
} msg_struct; 
 
int main(int argc, char *argv[])  
{ 
    int msg_id; 
    int status; 
    msg_struct msg; 
    int prize = 0; 
 
    // connect to the message queue 
    msg_id = msgget(1000, 0600 | IPC_CREAT); 
    exit_on_error(msg_id, "Creation/Connection"); 
 
   // receive a message (blocks if none) 
   while (1) { 
      status = msgrcv(msg_id, &msg, sizeof(msg.msg), 1, 0); 
      exit_on_error(status, "Reception"); 
 
      printf("From PID %d: <%s>\n", msg.msg.pid, msg.msg.text); 
      msg.type = msg.msg.pid; 
      if (++prize %5 == 0){ 
          strcpy(msg.msg.text, "PRIZE!"); 
          printf("Prize for PID =%d\n", msg.msg.pid); 
      } 
      else 
          strcpy (msg.msg.text, "Try again."); 
       
      status = msgsnd(msg_id, &msg, sizeof(msg.msg), 0); 
      exit_on_error(status, "Answer"); 
   } 
}