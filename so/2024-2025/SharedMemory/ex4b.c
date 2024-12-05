#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 


#include "semaphore.h"

 
//#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); } 
#define exit_on_null(s,m) if ( s == NULL ) { perror(m); exit(1); } 
 
int main(int argc, char *argv[])  
{ 
    int shm_id, status; 
    char *ptr; 
    int i; 
    int mutex;
     
    shm_id = shmget(1000, 800, 0600 | IPC_CREAT); 
    exit_on_error(shm_id, "Creation"); 
 
    ptr = (char *)shmat(shm_id, NULL, 0); 
    exit_on_null(ptr, "Attach"); 


    mutex = create_mutex(1000);


    while (1) { 
    printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n"); 
    down(mutex);
        for (i = 0; i < 800; i++) { 
 
            printf("%c", ptr[i]); 
            if ((i+1) % 80 == 0)  
                printf ("\n"); 
        } 
        up(mutex);
    printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n"); 
        sleep(1); 
   } 
}