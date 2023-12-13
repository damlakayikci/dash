#ifndef EXECUTE_H
#define EXECUTE_H

/**
 * @file execute.h
 * @brief Declarations of the executer functions for the project.
 * @author Damla Kayikci
 * @date 09/12/2023
 */

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
/**
 * Forks a child process and executes the given command.
 * If the output needs to be redirected, redirects the inputs to execute_redirect() function
 * @param path: The path of the command to execute.
 * @param args: The arguments to pass to the command.
 */

int bello(char **args, char *previous, int arg_count);
/**
    Custom command for the project.
    @param args: The arguments to pass to the command.
    @param previous: The previous command.
    @param arg_count: The number of arguments.
    @return 1 if the command is executed successfully, 0 otherwise.
*/

void echo(char *input, int length);
/**
    Special function to parse echo commands.
    @param input: The input to echo.
    @param length: The length of the input.
*/

char *string_in_reverse(char *input);
/**
    Reverses the given string.
    @param input: The string to reverse.
    @return The reversed string.
*/

#endif
