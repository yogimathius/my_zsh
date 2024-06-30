#include "../inc/main.h"

void open_shell(int ac, char** av, char** env) {
  // char* args[] = {"/bin/sh", NULL};
  // execv(args[0], args);
  int shell_exited = 0;
  if (ac > 1) {
    char** args = (char**)malloc(sizeof(char*) * (ac + 1));
    if (args == NULL) {
      perror("malloc");
      return;
    }

    for (int i = 0; i < ac - 1; i++) {
      args[i] = av[i + 1];
    }

    args[ac - 1] = NULL;
    shell_exited = execute_args(args, env);
    free(args);
  }
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
