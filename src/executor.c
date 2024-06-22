#include "../inc/main.h"

char* find_executable(const char* command) {
  char* path = getenv("PATH");
  if (path == NULL) {
    return NULL;
  }

  char* path_copy = strdup(path);
  if (path_copy == NULL) {
    return NULL;
  }

  char* token = strtok(path_copy, ":");
  while (token != NULL) {
    char* full_path = (char*)malloc(strlen(token) + strlen(command) + 2);
    if (full_path == NULL) {
      free(path_copy);
      return NULL;
    }

    snprintf(full_path, strlen(token) + strlen(command) + 2, "%s/%s", token, command);
    if (access(full_path, X_OK) == 0) {
      free(path_copy);
      return full_path;
    }

    free(full_path);
    token = strtok(NULL, ":");
  }

  free(path_copy);
  return NULL;
}

int execve(const char* filename, char* const argv[], char* const envp[]);

void run_command(char** args) {
  char* exec = find_executable(args[0]);
  if (exec != NULL) {
    execve(exec, args, NULL);
  }

  else if (is_builtin(args[0])) {
    char path[1024];
    snprintf(path, 1024, "/bin/%s", args[0]);
    execve(path, args, NULL);
  }
  else {
    printf("Command not found: %s\n", args[0]);
  }
  perror("execve");
  exit(1);
}