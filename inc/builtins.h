#ifndef BUILTINS_H
#define BUILTINS_H

int custom_cd(char** args);
int custom_exit(char** args);
int custom_env(char** args, char** env);
int custom_echo(char** args);
int custom_ls(char** args);
int custom_pwd(char** args);
int custom_setenv(char** args);
int custom_unsetenv(char** args);
int custom_which(char** args);

int run_builtin(char** args, char** env);

#endif