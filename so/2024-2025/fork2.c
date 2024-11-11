
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


void main(void) {
  int i, pid, ret;


  for (i = 1; i < 11; i++) {
    pid = fork();
    if (pid == 0) {
      printf("Sou o filho número %i (PID = %i)\n", i, getpid());
      exit(i + 10);
    }
    printf("Pai: lancei o filho %i com o PID %i\n", i, pid);
  }


  while ((pid = wait(&ret)) != -1) {
    ret = WEXITSTATUS(ret);
    printf("O filho com PID %i devolveu-me %i\n", pid, ret);
  }
}