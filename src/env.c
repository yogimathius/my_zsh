#include "../inc/main.h"

char** create_env(char** env) {
  int env_size = 0;
  while (env[env_size] != NULL) {
    env_size++;
  }

  char** new_env = (char**)malloc(sizeof(char*) * (env_size + 1));
  if (new_env == NULL) {
    perror("malloc");
    return NULL;
  }

  for (int i = 0; i < env_size; i++) {
    new_env[i] = strdup(env[i]);
    if (new_env[i] == NULL) {
      perror("strdup");
      for (int j = 0; j < i; j++) {
        free(new_env[j]);
      }
      free(new_env);
      return NULL;
    }
  }

  new_env[env_size] = NULL;
  return new_env;
}

void free_env(char** env) {
  if (env == NULL) return;
  for (int i = 0; env[i] != NULL; i++) {
    free(env[i]);
  }
  free(env);
}

