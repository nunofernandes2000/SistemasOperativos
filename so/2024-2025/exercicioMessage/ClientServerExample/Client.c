#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <unistd.h>

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
   char s[250]; 
 
   // connect to the message queue 
   msg_id = msgget(1000, 0600 | IPC_CREAT); 
   exit_on_error(msg_id, "Creation/Connection"); 
 
   printf("PID=%d\n", getpid()); 
   while (1) { 
      msg.type = 1; 
      msg.msg.pid = getpid(); 
      printf("Message: "); 
      fgets(s, 250, stdin); 
      strcpy(msg.msg.text, s); 
      status = msgsnd(msg_id, &msg, sizeof(msg.msg), 0); 
      exit_on_error(status, "Send"); 
 
      status = msgrcv(msg_id, &msg, sizeof(msg.msg), getpid(), 0); 
      exit_on_error(status, "Answer"); 
      printf("Answer: %s\n", msg.msg.text);
  } 
}