#include "../inc/main.h"

void open_shell(char** env) {
  UNUSED(env);
  // char* args[] = {"/bin/sh", NULL};
  // execv(args[0], args);
  int shell_exited = 0;

  while (!shell_exited) {
    char* input = (char*)malloc(1024);

    if (input == NULL) {
      perror("malloc");
      return;
    }

    get_input(input);
    char** args = split_input(input);
    new_process(args);
    free(input);
  }
}