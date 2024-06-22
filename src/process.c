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