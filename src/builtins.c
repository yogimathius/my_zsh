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

int custom_echo(char** args, char** env) {
  if (args[1][0] == '$') {
    int i = 0;
    while (env != NULL && env[i] != NULL) {
      if (strncmp(env[i], args[1] + 1, strlen(args[1]) - 1) == 0) {
        strtok(env[i], "=");
        char* value = strtok(NULL, "=");
        printf("%s\n", value);
        return 0;
      }
      i++;
    }
    return 0;
  }
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
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

int custom_setenv(char** args, char** env) {
  if (args[1] == NULL) {
    fprintf(stderr, "setenv: expected argument\n");
  }
  char* env_var = strtok(args[1], "=");
  char* value = strtok(NULL, "=");
  if (env_var == NULL) {
    fprintf(stderr, "setenv: expected argument in the form of VAR=VAL\n");
  }
  char* arg = value + 2;
  // slice off #{ and closing } that is in arg currently
  arg[strlen(arg) - 1] = '\0';

  int i = 0;
  // combine the env_var and arg
  char* new_env = (char*)malloc(strlen(arg) + strlen(env_var) + 2);
  if (new_env == NULL) {
    perror("malloc");
    return 1;
  }
  snprintf(new_env, strlen(env_var) + strlen(arg) + 2, "%s=%s", env_var, arg);
  while (env[i] != NULL) {
    if (strncmp(*env, env_var, strlen(env_var)) == 0) {
      *env = strdup(new_env);
      return 0;
    }
    i++;
  }

  env[i] = strdup(new_env);
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
    return custom_echo(args, env);
  }
  else if (strcmp(args[0], "pwd") == 0) {
    return custom_pwd(args);
  }
  else if (strcmp(args[0], "setenv") == 0) {
    return custom_setenv(args, env);
  }
  else if (strcmp(args[0], "unsetenv") == 0) {
    return custom_unsetenv(args);
  }
  else if (strcmp(args[0], "which") == 0) {
    return custom_which(args);
  }
  return 1;
}
