#ifndef BUILTINS_H
#define BUILTINS_H

int custom_cd(char** args);
int custom_exit(char** args);
int custom_env(char** args, char** env);
int custom_echo(char** args, char** env);
int custom_pwd(char** args);
int custom_setenv(char** args, char** env);
int custom_unsetenv(char** args, char** env);
int custom_which(char** args, char** env);

int run_builtin(char** args, char** env);

#endif
