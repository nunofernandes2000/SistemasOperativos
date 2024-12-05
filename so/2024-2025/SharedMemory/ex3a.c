#include <stdio.h> 
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
 
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); } 
#define exit_on_null(s,m) if ( s == NULL ) { perror(m); exit(1); } 
 
typedef char line[20]; 
 
int main(int argc, char *argv[]) 
{ 
   int shm_id, status; 
   line *ptr; 
   int i; 
 
   shm_id = shmget(1000, 800, 0600 | IPC_CREAT); 
   exit_on_error(shm_id, "Creation"); 
 
   ptr = (line *)shmat(shm_id, NULL, 0); 
   exit_on_null(ptr, "Attach"); 
 
   for (i = 0; i < 20; i++) { 
      sprintf(ptr[i], "line number %d", i+1); 
   } 
}