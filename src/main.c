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
    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
      shell_exited = 1;
    }
    else if (strcmp(input, "sh") == 0) {
      char* args[] = { "/bin/sh", NULL };
      execve("/bin/sh", args, NULL);
      perror("execve");
    }
    else if (strncmp(input, "ls", 2) == 0) {
      char* args[] = { "/bin/ls", NULL };
      execve("/bin/ls", args, NULL);
      perror("execve");
    }
    else if (strncmp(input, "cat", 3) == 0) {
      char* args[] = { "/bin/cat", NULL };
      execve("/bin/cat", args, NULL);
      perror("execve");
    }
    else {
      printf("Command not found: %s\n", input);
    }
    free(input);
  }
}

int main() {
  /* code */
  open_shell();
  return 0;
}
