#ifndef EXECUTE_H
#define EXECUTE_H

#include <unistd.h> // needed for fork(), exec()
#include <stdio.h> // needed for perror()
#include <sys/wait.h> // needed for wait()
#include <ctype.h> // needed for isspace()
#include <string.h> // needed for strlen(), strstr()
#include <stdlib.h> // needed for malloc(), free()

void execute(char *path, char **args);

void bello();

void echo(char *input, int length);

char* string_in_reverse(char *input);

void echo(char *input, int length);


#endif

