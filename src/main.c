#include "../inc/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// void execute_command(char* command) {

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
    if (strlen(input) > 0) {
      // add_history(input);
      // printf("You entered: %s\n", input);
    }
    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
      // printf("Exiting shell...\n");
      shell_exited = 1;
    }
    else {
      // execute_command(input);
    }
    free(input);
  }
}

int main() {
  /* code */
  open_shell();
  return 0;
}
