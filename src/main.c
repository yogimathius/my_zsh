#include "../inc/main.h"

void new_process(char** args) {
  if (args[0] == NULL) {
    return;
  }

  pid_t pid = fork();

  if (pid == 0) {
    run_command(args);
  }
  else if (pid > 0) {  // Parent process
    int status;
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  else {
    perror("fork");
  }
}

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

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  open_shell(env);
  return 0;
}
