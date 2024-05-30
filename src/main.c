#include "../inc/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// List of built-in commands
char* builtins[] = {
    "alias",
    "bg",
    "bind",
    "break",
    "builtin",
    "cd",
    "command",
    "compgen",
    "complete",
    "compopt",
    "continue",
    "declare",
    "dirs",
    "disown",
    "echo",
    "enable",
    "eval",
    "exec",
    "exit",
    "export",
    "false",
    "fc",
    "fg",
    "getopts",
    "hash",
    "help",
    "history",
    "jobs",
    "kill",
    "let",
    "local",
    "logout",
    "mapfile",
    "popd",
    "printf",
    "pushd",
    "pwd",
    "read",
    "readarray",
    "readonly",
    "return",
    "set",
    "shift",
    "shopt",
    "source",
    "suspend",
    "test",
    "times",
    "trap",
    "true",
    "type",
    "typeset",
    "ulimit",
    "umask",
    "unalias",
    "unset",
    "wait",
};

int is_builtin(const char* command) {
  size_t num_builtins = sizeof(builtins) / sizeof(builtins[0]);
  for (size_t i = 0; i < num_builtins; i++) {
    if (strcmp(command, builtins[i]) == 0) {
      return 1; // Command is a built-in
    }
  }
  return 0; // Command is not a built-in
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

void open_shell(char** env) {
  // char* args[] = {"/bin/sh", NULL};
  // execv(args[0], args);
  int shell_exited = 0;

  while (!shell_exited) {
    fputs("$ ", stdout);

    char* input = (char*)malloc(1024);
    if (input == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }

    if (fgets(input, 1024, stdin) == NULL) {
      free(input);
      break;
    }
    input[strcspn(input, "\n")] = 0;

    char* args[64];
    char* token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < 63) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) {
      free(input);
      continue;
    }

    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
      shell_exited = 1;
    }
    else {
      pid_t pid = fork();

      if (pid == 0) {
        char* exec = find_executable(args[0]);
        if (exec != NULL) {
          execve(exec, args, env);
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
    free(input);
  }
}

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  open_shell(env);
  return 0;
}
