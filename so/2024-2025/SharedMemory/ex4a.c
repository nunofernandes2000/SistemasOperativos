/*
 * Este programa em C cria e escreve em uma memória compartilhada.
 * 
 * Funcionalidade:
 * 1. Cria um segmento de memória compartilhada com uma chave de 1000 e tamanho de 800 bytes.
 * 2. Anexa o segmento de memória compartilhada ao espaço de endereço do processo.
 * 3. Calcula um caractere baseado no PID do processo e escreve esse caractere repetidamente na memória compartilhada.
 * 4. O programa entra em um loop infinito onde escreve o caractere na memória compartilhada e realiza uma operação de cálculo para consumir tempo.
 * 
 * Detalhes das Funções:
 * - shmget: Cria um segmento de memória compartilhada.
 * - shmat: Anexa o segmento de memória compartilhada ao espaço de endereço do processo.
 * - perror: Imprime uma mensagem de erro.
 * - exit: Encerra o programa em caso de erro.
 * 
 * Macros:
 * - exit_on_error: Verifica se uma operação retornou um valor negativo e, em caso afirmativo, imprime uma mensagem de erro e encerra o programa.
 * - exit_on_null: Verifica se um ponteiro é NULL e, em caso afirmativo, imprime uma mensagem de erro e encerra o programa.
 * 
 * Variáveis:
 * - shm_id: Identificador do segmento de memória compartilhada.
 * - status: Variável de status (não utilizada).
 * - ptr: Ponteiro para o segmento de memória compartilhada.
 * - i, k: Variáveis de loop.
 * - c: Caractere a ser escrito na memória compartilhada.
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 

#include "semaphore.h"  // inclui o código do semaphore.h (arquivo com as funções para manipular semaforos)
 
//#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); } 
#define exit_on_null(s,m) if ( s == NULL ) { perror(m); exit(1); } 
 
int main(int argc, char *argv[])  
{ 
   int shm_id, status; 
   char *ptr; 
   int i, k; 
   int c; 
   int mutex;
 
   shm_id = shmget(1000, 800, 0600 | IPC_CREAT); 
   exit_on_error(shm_id, "Creation"); 
 
   ptr = (char *)shmat(shm_id, NULL, 0); 
   exit_on_null(ptr, "Attach"); 

   mutex = create_mutex(1000); // criamos um semaforo com a chave 1000 e inicializamos com 1


   c = 'A' + getpid() % 26; // obtemos um nº entre 0 e 25 e somamos 'A' para obter um caractere entre 'A' e 'Z'
   printf("Writing '%c'\n",c);


   while (1) { 
    down(mutex);
      for (i = 0; i < 800; i++) { 
         ptr[i] = c; 
            for (k = 0; k < 1000000; k++) //este 2º for é para consumir tempo (causa um delay)
               k = k * 2.0 / 2.0; 
      } 
      up(mutex);
   } 
}