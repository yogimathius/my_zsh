#include "../inc/main.h"

void open_shell(char** env) {
  // char* args[] = {"/bin/sh", NULL};
  // execv(args[0], args);
  int shell_exited = 0;

  do {
    char* input = (char*)malloc(1024);

    if (input == NULL) {
      perror("malloc");
      return;
    }

    get_input(input);
    char** args = split_input(input);
    shell_exited = execute_args(args, env);
    free(input);
  } while (!shell_exited);
}