#include "execute.h"
#include "helper.h"
#include <stdio.h>  // needed for printf()
#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()

int main() {
    // initialize variables
    int path_count;   // number of paths in PATH
    int arg_count;    // number of arguments
    char *paths[100]; // array of paths in PATH
    char *args[40];   // array of arguments
    char *path;       // PATH variable to be parsed
    char *path_token;
    char *command;  // command to be executed
    int comparison; // comparison of exit and command

    // allocate memory
    command_found = malloc(sizeof(int));

    do {
        // initialize variables
        *command_found = 0;
        found_path = (char *)malloc(1024 * sizeof(char));
        arg_count = 0;

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< PATH variable --------------------------------------
        char *original_path = strdup(getenv("PATH")); // !!! PATH burada farkli sirada geliyor

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
        char *original_input = strdup(input); // save original input for echo
        printf("Length of input is %lu\n", strlen(input));
        char *token = input;
        token = trim(token);
        printf("Trimmed input is %s\n", token);

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
        comparison = strcmp(command, "exit");

        // if command is not exit
        if (comparison) {
            // <<<<<<<<<<<<< echo ----------------
            if (strcmp(command, "echo") == 0) {
                echo(original_input, strlen(original_input));
                // free memory
                free(found_path);
                continue;
            }
            // ------------- echo >>>>>>>>>>>>>>>>
            if (strcmp(command, "alias") == 0) {
                alias(original_input, strlen(original_input));
                // free memory
                free(found_path);
                continue;
            }
            // parse input
            while (token != NULL) {
                token = trim(token);
                if (strlen(token) != 0) {
                    args[arg_count++] = token;
                }
                token = strtok(NULL, " ");
            }

            // null terminate args
            args[arg_count] = NULL;

            // ------------------------------------------ Input >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

            // <<<<<<<<<<<<< bello ----------------
            if (strcmp(command, "bello") == 0) {
                bello();
                // free memory
                free(found_path);
                continue;
            }
            // ------------- bello >>>>>>>>>>>>>>>>

            findCommandInPath(command, paths, path_count);

            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Execute ------------------------------------------
            if (*command_found) {

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

                // ------------------------------------------ Execute >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            } else {
                fprintf(stderr, "%s: command not found\n", command);
            }
        }

        // free memory
        free(found_path);

    } while (comparison);

    free(command_found);

    return 0;
}