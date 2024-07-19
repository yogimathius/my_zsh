#include "../inc/main.h"
#include <signal.h>
#include <stdio.h>

void sigsegv_handler(int sig) {
  UNUSED(sig);
  printf("segmentation fault");
  exit(1); // Exit the program with an error code
}

char* get_env(char** env, const char* key) {
  for (size_t i = 0; env[i] != NULL; i++) {
    if (strncmp(env[i], key, strlen(key)) == 0) {
      return env[i] + strlen(key) + 1;
    }
  }
  return NULL;
}

char* find_executable(const char* command, char** env) {
  if (access(command, X_OK) == 0) {
    // Command is executable, return a copy of the command
    return strdup(command);
  }

  char* path = get_env(env, "PATH");
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

int run_command(char** args, char** env) {
  char* exec = find_executable(args[0], env);
  if (exec != NULL) {
    return  execve(exec, args, NULL);
  }

  else {
    write(STDERR_FILENO, "Command not found\n", 18);
    return 1;
  }
  perror("execve");
  return 1;
}

int execute_args(char** args, char** env) {
  if (args[0] == NULL) {
    return 1;
  }

  if (is_builtin(args[0])) {
    char path[1024];
    snprintf(path, 1024, "/bin/%s", args[0]);
    // execve(path, args, NULL);
    return run_builtin(args, env);
  }

  pid_t pid = fork();

  if (pid == 0) {
    return run_command(args, env);
  }
  else if (pid < 0) {   // Error forking
    perror("fork");
    return EXIT_FAILURE;
  }
  else {
    int status; // Parent process
    waitpid(pid, &status, WUNTRACED);

    if (WIFEXITED(status)) { return WEXITSTATUS(status); }  // Child exited normally
    else if (WIFSIGNALED(status)) {     // Child terminated by a signal
      int term_sig = WTERMSIG(status);
      printf("seg fault");
      sigsegv_handler(term_sig);
      return EXIT_FAILURE;
    }
  }
  return 0;
}
