#include "../inc/main.h"

char* builtins[] = {
    "cd",
    "echo",
    "exit",
    "env",
    "setenv",
    "unsetenv",
    "pwd",
    "which",
    "ls",
};

int is_builtin(const char* command) {
  size_t num_builtins = sizeof(builtins) / sizeof(builtins[0]);
  for (size_t i = 0; i < num_builtins; i++) {
    if (strcmp(command, builtins[i]) == 0) {
      return 1;
    }
  }
  return 0;
}
