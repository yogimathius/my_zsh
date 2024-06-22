#include "types.h"
#include "zsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef MAIN_H
#define MAIN_H

void get_input(char* input);
char** split_input(char* input);

int is_builtin(const char* command);

int run_command(char** args);

int execute_args(char** args);

void open_shell(char** env);

#endif // MAIN_H