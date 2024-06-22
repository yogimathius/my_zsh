#include "../inc/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// List of built-in commands
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
    "quit"
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

// void execute_command(char* command) {
int execve(const char* filename, char* const argv[], char* const envp[]);

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

void execute_command(char** args) {
  if (args[0] == NULL) {
    return;
  }

  pid_t pid = fork();

  if (pid == 0) {
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
    execute_command(args);
    free(input);
  }
}

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  open_shell(env);
  return 0;
}
