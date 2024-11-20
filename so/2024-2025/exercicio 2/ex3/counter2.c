/*
 * thread.c - counter thread
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N   10

int Counter = 0;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

void* thr_incrementer(void* ptr)
{
   int id = (int)ptr;

   printf("Thread %d started\n", id);
   while (1) {
      pthread_mutex_lock(&Mutex);
      if (Counter % N == id) {
         Counter++;
         printf("[%02d] Counter=%d\n", id, Counter);
         sleep(1);
      }
      pthread_mutex_unlock(&Mutex);
   }
   return NULL;
}

int main()
{
    int i;
    pthread_t tid[N];

    //pthread_mutex_init(&Mutex, NULL);

    /* create threads */
    for (i = 0; i < N; i++) {
        if (pthread_create(&tid[i], NULL, thr_incrementer, (void *)i) != 0) {
            printf("Error creating thread.\n");
            return -1;
        }
    }
   
    /* wait for thread termination */
    for (i = 0; i < N; i++) {
        if (pthread_join(tid[i], NULL) != 0) {
            printf("Error joining thred.\n");
            return -1;
        }
    }

    printf("Finished: Counter=%d\n",Counter);
    return 0;
}