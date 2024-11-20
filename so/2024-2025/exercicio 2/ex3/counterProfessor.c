/*
 * thread.c - counter thread
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


int Counter = 0;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

void* thr_inc_even(void* ptr)
{
   while (1) {
      pthread_mutex_lock(&Mutex);
      if (Counter % 2 == 0) {
         Counter++;
         printf("[E] Counter=%d\n", Counter);
         sleep(1);
      }
      pthread_mutex_unlock(&Mutex);
   }
   return NULL;
}

void* thr_inc_odd(void* ptr)
{
   while (1) {
      pthread_mutex_lock(&Mutex);
      if (Counter % 2 != 0) {
         Counter++;
         printf("[O] Counter=%d\n", Counter);
         sleep(1);
      }
      pthread_mutex_unlock(&Mutex);
   }
   return NULL;
}

int main()
{
   pthread_t tid_even, tid_odd;

   //pthread_mutex_init(&Mutex, NULL);

   if (pthread_create(&tid_even, NULL, thr_inc_even, NULL) != 0) {
      printf("Error creating thread.\n");
      return -1;
   }
   if (pthread_create(&tid_odd, NULL, thr_inc_odd, NULL) != 0) {
      printf("Error creating thread.\n");
      return -1;
   }
   
   if(pthread_join(tid_even, NULL) != 0) {
      printf("Error joining thred.\n");
      return -1;
   }
   if(pthread_join(tid_odd, NULL) != 0) {
      printf("Error joining thred.\n");
      return -1;
   }

   printf("Finished: Counter=%d\n",Counter);
   return 0;
}