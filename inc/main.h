#include "types.h"
#include "zsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "builtins.h"
#include "zsh.h"

#ifndef MAIN_H
#define MAIN_H

void get_input(char* input);
char** split_input(char* input);

int is_builtin(const char* command);

char* find_executable(const char* command, char** env);
int run_command(char** args, char** env);

int execute_args(char** args, char** env);
void free_args(char** args);
void open_shell(int ac, char** av, char** env);

#endif // MAIN_H
