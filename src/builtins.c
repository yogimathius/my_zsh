#include "../inc/main.h"

const char* builtins[] = {
    "cd",
    "echo",
    "quit",
    "env",
    "setenv",
    "unsetenv",
    "pwd",
    "which",
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

int custom_cd(char** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "cd: expected argument to \"cd\"\n");
  }
  else {
    if (chdir(args[1]) != 0) {
      perror("cd");
    }
  }
  return 0;

}
int custom_exit(char** args) {
  UNUSED(args);
  return 1;
}

int custom_env(char** args, char** env) {
  UNUSED(args);
  while (*env) {
    write(1, *env, strlen(*env));
    env++;
  }
  return 0;
}

int custom_echo(char** args) {
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
  return 1;

}

int custom_ls(char** args) {
  UNUSED(args);
  const char* path = ".";
  DIR* dir = opendir(path);
  if (dir == NULL) {
    perror("ls");
    return 0;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
  return 0;

}

int custom_pwd(char** args) {
  UNUSED(args);
  char path[1024];
  if (getcwd(path, 1024) == NULL) {
    perror("pwd");
    return 1;
  }
  printf("%s\n", path);
  return 0;


}

int custom_setenv(char** args) {
  if (args[1] == NULL || args[2] == NULL) {
    fprintf(stderr, "setenv: expected two arguments\n");
  }
  else {
    if (setenv(args[1], args[2], 1) != 0) {
      perror("setenv");
    }
  }
  return 0;


}

int custom_unsetenv(char** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "unsetenv: expected argument\n");
  }
  else {
    if (unsetenv(args[1]) != 0) {
      perror("unsetenv");
    }
  }
  return 0;
}

int custom_which(char** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "which: expected argument\n");
    return 1;
  }

  char* exec = find_executable(args[1]);
  if (exec != NULL) {
    printf("%s\n", exec);
    free(exec);
  }
  else {
    printf("Command not found: %s\n", args[1]);
    return 1;
  }
  return 0;
}

int run_builtin(char** args, char** env) {
  if (strcmp(args[0], "cd") == 0) {
    return custom_cd(args);
  }
  else if (strcmp(args[0], "quit") == 0) {
    return custom_exit(args);
  }
  else if (strcmp(args[0], "env") == 0) {
    return custom_env(args, env);
  }
  else if (strcmp(args[0], "echo") == 0) {
    return custom_echo(args);
  }
  else if (strcmp(args[0], "ls") == 0) {
    return custom_ls(args);
  }
  else if (strcmp(args[0], "pwd") == 0) {
    return custom_pwd(args);
  }
  else if (strcmp(args[0], "setenv") == 0) {
    return custom_setenv(args);
  }
  else if (strcmp(args[0], "unsetenv") == 0) {
    return custom_unsetenv(args);
  }
  else if (strcmp(args[0], "which") == 0) {
    return custom_which(args);
  }
  return 1;
}
