#ifndef EXECUTE_H
#define EXECUTE_H

#include <unistd.h> // needed for fork(), exec()
#include <stdio.h> // needed for perror()
#include <sys/wait.h> // needed for wait()

int execute(char *path, char **args);

#endif

