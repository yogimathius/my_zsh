#include "../inc/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// void execute_command(char* command) {
int execve(const char* filename, char* const argv[], char* const envp[]);

void open_shell() {
  // char* args[] = {"/bin/sh", NULL};
  // execv(args[0], args);
  int shell_exited = 0;

  while (!shell_exited) {
    char* input = (char*)malloc(1024);
    if (input == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }

    printf("$ ");
    if (fgets(input, 1024, stdin) == NULL) {
      free(input);
      break;
    }
    input[strcspn(input, "\n")] = 0;

    char* args[64];
    char* token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < 63) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) {
      free(input);
      continue;
    }

    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
      shell_exited = 1;
    }
    else {
      pid_t pid = fork();

      if (pid == 0) {  // Child process
        if (strcmp(args[0], "ls") == 0) {
          execve("/bin/ls", args, NULL);
        }
        else if (strcmp(args[0], "cat") == 0) {
          execve("/bin/cat", args, NULL);
        }
        else if (strcmp(args[0], "sh") == 0) {
          execve("/bin/sh", args, NULL);
        }
        else {
          printf("Command not found: %s\n", args[0]);
        }
        perror("execve");  // execve failed
        exit(1);
      }
      else if (pid > 0) {  // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for child process to finish
      }
      else {
        perror("fork");
      }
    }
    free(input);
  }
}

int main() {
  /* code */
  open_shell();
  return 0;
}
