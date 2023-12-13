#ifndef EXECUTE_H
#define EXECUTE_H

#include <ctype.h>    // needed for isspace()
#include <fcntl.h>    // needed for open, O_WRONLY, O_CREAT, O_APPEND, O_TRUNC
#include <stdio.h>    // needed for perror()
#include <stdlib.h>   // needed for malloc(), free()
#include <string.h>   // needed for strlen(), strstr()
#include <sys/wait.h> // needed for wait()
#include <time.h>     // needed for time()
#include <unistd.h>   // needed for fork(), exec()

// Global variables
extern int current_processes;

void execute(char *path, char **args);

int bello(char **args, char *previous, int arg_count);

void echo(char *input, int length);

char *string_in_reverse(char *input);

void echo(char *input, int length);

void alias(char *input, int length);

#endif
