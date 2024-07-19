#include "../inc/main.h"

#define MAX_ARGS 64

void get_input(char* input) {
  if (input == NULL) return;
  if (fgets(input, 1024, stdin) == NULL) {
    perror("fgets");
    return;
  }
  input[strcspn(input, "\n")] = 0;
}

char** split_input(char* input) {
  if (input == NULL) return NULL;

  char** args = (char**)malloc(MAX_ARGS * sizeof(char*));
  if (args == NULL) {
    perror("malloc");
    return NULL;
  }

  char* token = strtok(input, " ");
  int i = 0;
  while (token != NULL) {
    args[i] = strdup(token);
    if (args[i] == NULL) {
      perror("strdup");
      // Free already allocated memory
      for (int j = 0; j < i; j++) {
        free(args[j]);
      }
      free(args);
      return NULL;
    }
    i++;
    token = strtok(NULL, " ");
  }
  args[i] = NULL;
  return args;
}


void free_args(char** args) {
  if (args == NULL) return;
  for (int i = 0; args[i] != NULL; i++) {
    free(args[i]);
  }
  free(args);
}
