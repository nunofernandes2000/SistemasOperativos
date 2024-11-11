
/*
Nuno Fernandes --> 21635
Rafael Carvalho --> 21986
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_CMD 100

typedef struct
{
  char *cmd;
  char *linux_command;
  char *help;
} COMMAND;

COMMAND v_commands[] = {
    {"clear", "clear", "Clear the terminal screen"},
    {"list", "ls", "List directory contents"},
    {"who", "who", "Show who is logged on"},
    {"processes", "top", "Display Linux processes"},
    {"exit", NULL, "Quit my shell"},
    {NULL, NULL, NULL} 
};

// Função para encontrar o comando correspondente
COMMAND* find_command(char *cmd) {
  for (int i = 0; v_commands[i].cmd != NULL; i++) {
    if (strcmp(v_commands[i].cmd, cmd) == 0) {
      return &v_commands[i];
    }
  }
  return NULL;
}


void argCommand(char * user_input) {
  char * arg[10]; // Array para argumentos
  char * token; // Token
  int index = 0;
  int pid;

  // Divide a entrada em tokens usando o espaço
  token = strtok(user_input, " ");
  while (token != NULL) {
    arg[index++] = token;
    token = strtok(NULL, " ");
  }
  arg[index] = NULL;

  // Encontra o comando correspondente
  COMMAND *command = find_command(arg[0]);
  if (command == NULL) {
    printf("Comando não reconhecido\n");
    return;
  }

  // Se o comando for "exit", o loop é encerrado
  if (strcmp(command->cmd, "exit") == 0) {
    printf("BYE BYE USER...\n");
    exit(0);
  }

  // Cria um processo filho para executar o comando
  pid = fork();

  if (pid == 0) {
    // Ajusta o comando para o comando do Linux correspondente
    if (command->linux_command != NULL) {
      arg[0] = command->linux_command;
    }

    // Executa o comando
    execvp(arg[0], arg);
    // Se execvp falhar, exibe uma mensagem de erro e encerra o processo filho
    printf("ERROR: comando não encontrado\n");
    exit(1);
  } else if (pid > 0) { // No processo pai
    // Espera o processo filho terminar
    int status;
    wait(&status); // Espera a conclusão do processo filho
    if (WIFEXITED(status)) {
      printf("Processo filho terminou com status %d\n", WEXITSTATUS(status)); // Status 0 significa sucesso
    }
  } else {
    // Se fork falhar, exibe uma mensagem de erro
    printf("ERROR: falha no fork\n");
    exit(1);
  }
}

void print_commands() {
  printf("\n---------------------------\n");
  printf("Comandos disponíveis:\n");
  for (int i = 0; v_commands[i].cmd != NULL; i++) {
    printf("%d. %s (%s) - %s\n", i + 1, v_commands[i].cmd, v_commands[i].linux_command ? v_commands[i].linux_command : "N/A", v_commands[i].help);
  }
  printf("---------------------------\n");
}

int main(void) {
  char user_input[MAX_CMD + 1]; // Buffer para armazenar o comando

  printf("Welcome\n");

  while (1) {
    // Exibe a lista de comandos
    print_commands();

    // Solicita o comando ao usuário
    printf("By your command: ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = 0;

    argCommand(user_input);
  }

  return 0;
}