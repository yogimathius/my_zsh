#include "../inc/main.h"

void get_input(char* input) {
  write(STDOUT_FILENO, "", strlen(""));
  if (fgets(input, 1024, stdin) == NULL) {
    return;
  }
  input[strcspn(input, "\n")] = 0;
}

char** split_input(char* input) {
  char** args = (char**)malloc(64 * sizeof(char*));
  if (args == NULL) {
    perror("malloc");
    return NULL;
  }

  char* token = strtok(input, " ");
  int i = 0;
  while (token != NULL) {
    args[i] = token;
    i++;
    token = strtok(NULL, " ");
  }
  args[i] = NULL;
  return args;
}