#ifndef EXECUTE_H
#define EXECUTE_H

#include <ctype.h>    // needed for isspace()
#include <stdio.h>    // needed for perror()
#include <stdlib.h>   // needed for malloc(), free()
#include <string.h>   // needed for strlen(), strstr()
#include <sys/wait.h> // needed for wait()
#include <unistd.h>   // needed for fork(), exec()
#include <fcntl.h>    // needed for open, O_WRONLY, O_CREAT, O_APPEND, O_TRUNC


// Global variables
extern char *previous;
extern int current_processes;

void execute(char *path, char **args);

void bello();

void echo(char *input, int length);

char *string_in_reverse(char *input);

void echo(char *input, int length);

void alias(char *input, int length);

#endif
