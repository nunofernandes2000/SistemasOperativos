/*
 * thread.c - simple example demonstrating the creation of threads
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for sleep



int Value = 0;

/**
 * @brief Função da thread que define a variável global Value para 1.
 *
 * Esta função é destinada a ser executada por uma thread recém-criada.
 * Ela atribui o valor 1 à variável global `Value` e então termina.
 *
 * @param ptr Um ponteiro para quaisquer dados passados para a função da thread (não utilizado neste caso).
 * @return Sempre retorna NULL.
 */
void* thr_func(void* ptr)
{
   Value = 1;
   return NULL;
}

/**
 * @brief Função principal do programa que cria e gerencia uma thread.
 *
 * Esta função cria uma thread utilizando a função pthread_create, aguarda a conclusão
 * da thread com pthread_join, e imprime o valor de uma variável compartilhada.
 * Em caso de erro na criação ou junção da thread, a função retorna -1.
 *
 * @return Retorna 0 em caso de sucesso, ou -1 em caso de erro.
 */
int main()
{
   pthread_t tid; // identificador da thread
   if (pthread_create(&tid, NULL, thr_func, NULL) != 0) { 
      printf("Error creating thread.\n");
      return -1;
   }
   
   sleep(1); // sleep for 1 second
   Value = 2;
   
   if(pthread_join(tid, NULL) != 0) { //o pthread_join espera a thread terminar ou seja é parecido ao wait
      printf("Error joining thred.\n");
      return -1;
   }
	
   printf("Value=%d\n",Value);
   return 0;
}


//pode ser 1 ou 2 porque o valor depende do  escalanador do sistema operacional