#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define MAX_LINE 1024

void type_prompt();
void read_command(char *command, char **parameters);

int main() {
  char command[MAX_LINE];
  char *parameters[MAX_LINE];

  while (TRUE) {
    type_prompt();
    read_command(command, parameters);

    pid_t pid = fork();

    if (pid == 0) {
      if (execvp(command, parameters) == -1) {
        perror("Erro na execução do comando");
        exit(1);
      }
    } else if (pid > 0) {
      wait(NULL);
    } else {
      perror("Erro na criação do processo filho");
      exit(1);
    }
  }

  return 0;
}

void type_prompt() {
  char shell[MAX_LINE];
  if (getcwd(shell, MAX_LINE) != NULL) {
    printf("%s$ ", shell);
  } else {
    perror("Erro ao obter o diretório atual");
  }
}

void read_command(char *command, char **parameters) {
  char line[MAX_LINE];
  fgets(line, MAX_LINE, stdin);
  line[strcspn(line, "\n")] = 0;

  char *token = strtok(line, " ");
  int i = 0;
  while (token != NULL) {
    parameters[i++] = token;
    token = strtok(NULL, " ");
  }
  parameters[i] = NULL; 
  strcpy(command, parameters[0]);
}