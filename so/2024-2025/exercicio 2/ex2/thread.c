/*
 * thread.c - simple demonstration of mutual exclusion problems
 */

/**
 * Este programa demonstra o uso de threads em C utilizando a biblioteca pthread.
 *
 * Arquivo: thread.c
 * Caminho: /Users/nunofernandes/Desktop/SistemasOperativos/so/2024-2025/exercicio 2/ex2/thread.c
 *
 * Descrição:
 * - Declara uma variável global 'Value' inicializada com 10.
 * - Define a função 'thr_func' que será executada por cada thread:
 *      - Bloqueia o mutex antes de acessar 'Value'.
 *      - Verifica se 'Value' é maior ou igual a 10.
 *      - Se a condição for verdadeira, espera 1 segundo e subtrai 10 de 'Value'.
 *      - Desbloqueia o mutex após acessar 'Value'.
 * - Na função 'main':
 *      - Declara duas variáveis do tipo pthread_t para as threads 't1' e 't2'.
 *      - Inicializa o mutex.
 *      - Cria a primeira thread 't1' que executa 'thr_func'. Em caso de erro, exibe uma mensagem e encerra o programa.
 *      - Cria a segunda thread 't2' de forma similar à primeira.
 *      - Aguarda a conclusão das duas threads utilizando 'pthread_join'. Em caso de erro, exibe uma mensagem e encerra o programa.
 *      - Após a finalização das threads, imprime o valor final de 'Value'.
 *
 * Objetivo:
 * - Demonstrar a criação, execução e sincronização de threads em C.
 * - Mostrar como múltiplas threads podem alterar uma variável global de forma concorrente utilizando mutex para evitar condições de corrida.
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for sleep


int Value = 10;
pthread_mutex_t Mutex;

void* thr_func(void* ptr)
{
   pthread_mutex_lock(&Mutex); // lock the mutex before accessing 'Value'
   if (Value >= 10) {
      sleep(1); // to make the problem more likely to happen
      Value -= 10;
   }
   pthread_mutex_unlock(&Mutex); // unlock the mutex after accessing 'Value'
   return NULL;
}

int main()
{
   pthread_t t1,t2;
   pthread_mutex_init(&Mutex, NULL); // initialize the mutex -- é necessario sempre antes dos pthread_create
   
   if (pthread_create(&t1, NULL, thr_func, NULL) != 0) {
      printf("Error creating thread.\n");
      return -1;
   }
   if (pthread_create(&t2, NULL, thr_func, NULL) != 0) {
      printf("Error creating thread.\n");
      return -1;
   }

   if(pthread_join(t1, NULL) != 0) {
      printf("Error joining thred.\n");
      return -1;
   }
   if(pthread_join(t2, NULL) != 0) {
      printf("Error joining thred.\n");
      return -1;
   }

   printf("Value=%d\n",Value);
   return 0;
}

//Foi garantido que o resultado é sempre 0, pois o mutex garante que apenas uma thread pode aceder à variável global 'Value' de cada vez.