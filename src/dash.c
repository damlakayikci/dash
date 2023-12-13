#include "../headers/alias.h"
#include "../headers/execute.h"
#include "../headers/helper.h"
#include <signal.h> // needed for signal()
#include <stdio.h>  // needed for printf()
#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()

void sigintHandler(int sig_num) {
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}

int main() {
    signal(SIGINT, sigintHandler);
    // initialize variables
    int path_count;   // number of paths in PATH
    int arg_count;    // number of arguments
    char *paths[100]; // array of paths in PATH
    char *args[40];   // array of arguments
    char *path;       // PATH variable to be parsed
    char *path_token;
    char *command;       // command to be executed
    char *previous = ""; // previous command
    int current_processes;
    AliasArray alias_array;
    initArray(&alias_array, 20);

    // allocate memory
    command_found = malloc(sizeof(int));

    do {

        // initialize variables
        *command_found = 0;
        found_path = (char *)malloc(1024 * sizeof(char));
        arg_count = 0;

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< PATH variable --------------------------------------
        char *original_path = strdup(getenv("PATH"));

        // copy path to a new variable to avoid strtok() changing it
        path = strdup(original_path);

        // Parse PATH
        path_token = strtok(path, ":");
        path_count = 0;
        while (path_token != NULL) {
            paths[path_count++] = path_token;
            path_token = strtok(NULL, ":");
        }
        free(original_path);
        // -------------------------------------- PATH variable >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        print_command_prompt();

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Input ------------------------------------------
        // Get input script
        char input[1024];
        fgets(input, sizeof(input), stdin);

        // trim input
        char *original_input = trim(strdup(input)); // save original input for echo
        printf("Length of input is %lu\n", strlen(input));
        char *token = input;
        token = trim(token);
        printf("Trimmed input is %s\n", token);

        // check if input is an alias
        int alias_index = find_alias(&alias_array, token);
        if (alias_index != -1) {
            token = strdup(alias_array.array[alias_index].command);
            original_input = strdup(token);
        }

        // if token length is 0, there is no input. Ignore the rest
        if (strlen(token) == 0) {
            // free memory
            free(found_path);
            continue;
        }

        // tokenize input
        token = strtok(token, " ");

        // get the first token as command and check if it is exit
        command = trim(token);

        // if command is not exit
        if (strcmp(command, "exit")) {
            // <<<<<<<<<<<<< echo ----------------
            if (strcmp(command, "echo") == 0) {
                printf("Original input is %s\n", original_input);
                echo(original_input, strlen(original_input));
                // free memory
                free(found_path);
                continue;
            }
            // ------------- echo >>>>>>>>>>>>>>>>
            // <<<<<<<<<<<<< alias ----------------
            if (strcmp(command, "alias") == 0) {
                check_alias(&alias_array, original_input, strlen(original_input));
                // free memory
                free(found_path);
                continue;
            }
            // ------------- alias >>>>>>>>>>>>>>>>
            // parse input
            while (token != NULL) {
                token = trim(token);
                if (strlen(token) != 0) {
                    args[arg_count++] = token;
                    printf("Argument %d is %s\n", arg_count, token);
                }
                token = strtok(NULL, " ");
            }

            // null terminate args
            args[arg_count] = NULL;

            // ------------------------------------------ Input >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

            findCommandInPath(command, paths, path_count);

            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Execute ------------------------------------------
            // <<<<<<<<<<<<< bello ----------------
            if (previous == NULL) {
                printf("previous is null\n");
                previous = malloc(1);
                previous[0] = '\0';
            } else {
                printf("previous is %s\n", previous);
            }
            if (strcmp(command, "bello") == 0) {
                if (!bello(args, previous, arg_count)) {
                    previous = strdup("bello");
                }
                // free memory
                free(found_path);
                continue;

            }
            // ------------- bello >>>>>>>>>>>>>>>>
            else if (*command_found) {

                // <<<<<<<<<<<<<<<<< cd ---------------------
                if (strcmp(command, "cd") == 0) {
                    if (arg_count > 2) {
                        fprintf(stderr, "cd: too many arguments\n");
                    } else if (arg_count == 2) {
                        int result = chdir(args[1]);
                        if (result == -1) {
                            fprintf(stderr, "cd: no such file or directory: %s\n", args[1]);
                        }
                    } else {
                        char *home = getenv("HOME");
                        int result = chdir(home);
                        if (result == -1) {
                            fprintf(stderr, "cannot get environment variable HOME\n");
                        }
                    }
                    // free memory
                    free(found_path);
                    continue;
                }

                // ---------------- cd >>>>>>>>>>>>>>>>>>>>>>

                execute(found_path, args);
                previous = strdup(command);
                // ------------------------------------------ Execute >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            } else {
                fprintf(stderr, "%s: command not found\n", command);
            }

        } else {
            freeArray(&alias_array);
        }
        // free memory
        free(found_path);
        // while (waitpid(-1, NULL, WNOHANG) > 0) {
        //     // A periodic check on zombie processes
        // }

    } while (strcmp(command, "exit"));

    free(command_found);

    return 0;
}