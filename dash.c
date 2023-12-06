#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()
#include <stdio.h> // needed for printf()
#include <unistd.h> // needed for fork(), exec()
#include "find_command.h"
#include "trim.h"

// Global variables
int *command_found; // flag for command found
char *found_path; // path of the command found


int main(){
    // initialize variables
    int path_count;    // number of paths in PATH
    int arg_count;     // number of arguments
    char *paths[100];  // array of paths in PATH
    char *args[40];    // array of arguments
    char *path;        // PATH variable to be parsed
    char *path_token;  
    char *command;     // command to be executed
    int comparison;    // comparison of exit and command

    // allocate memory
    command_found = malloc(sizeof(int));

    do{ 
        // initialize variables
        *command_found = 0;
        found_path = (char *) malloc(1024 * sizeof(char));

        // <<<<<<<<<<<<<<<<<< PATH variable ------------------
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
        // ------------------ PATH variable >>>>>>>>>>>>>>>>>>


        // Hardcoded for now TODO : change this
        printf("damlakayikci@Damlas-MacBook-Pro.local /Users/damlakayikci/Desktop/cmpe/okul/cmpe322/shell-project --- ");
        

        // <<<<<<<<<<<<<<<<<<<<<< Input ----------------------
        // Get input script
        char input[1024];
        fgets(input, sizeof(input), stdin);

        // parse input
        char *token = input;
        token = trim(token);
      

        // if token length is 0, there is no input
        if (strlen(token) == 0) {
            printf("________________ No input\n");
            continue;
        }

        token = strtok(token, " ");
        printf("Input after2 is %s\n", token);
        arg_count = 0;

        // get the first token as command and check if it is exit
        command = token;
        printf("Command before is %s\n", command);
        command = trim(command);
        printf("Command after is %s\n", command);
        comparison = strcmp(command, "exit"); 
        printf("Comparison is %d\n", comparison);


        // if command is not exit
        if (comparison) {
            while (token != NULL) {
                // printf("Token is %s\n", token);
                token = trim(token);
                // if length of token is 0, ignore it
                if (strlen(token) != 0) {
                    args[arg_count++] = token;
                }
                token = strtok(NULL, " ");
            }

            // TODO : remove this
            printf("Arg count is %d\n", arg_count);
            for (int i = 0; i < arg_count; i++) {
                printf("Token %d is %s\n", i, args[i]);
                printf("Length of token %d is %lu\n", i, strlen(args[i]));
            }
            // TODO : remove this

        
            // ---------------------- Input >>>>>>>>>>>>>>>>>>>>>>
               
            // traverse through paths
            for (int i = 0; i < path_count; i++) {
                // find command in path
                // printf("Searching in %s\n", paths[i]);
                findFilesRecursively(paths[i], command);
                if (*command_found) {
                    // TODO : remove this
                    printf("Command found in %s\n", paths[i]);
                    printf("Command is %s\n", found_path);
                    // TODO : remove this
                    break;
                }
            }
            // <<<<<<<<<<<<<<<<<<<<<<< Execute -------------------
            // pid_t pid;
            // pid = fork();

            // if (pid < 0)
            // {
            //     fprintf(stderr, "Fork Failed");
            //     return 1;
            // }
            // else if (pid == 0)
            // {
            //     execlp(found_path, command, NULL);
            // }
            // else
            // {
            //     wait(NULL);
            // }
            // ----------------------- Execute >>>>>>>>>>>>>>>>>>>>


        }
        

        // free memory
        free(found_path);
        free(original_path);

     
    } while(comparison);

    free(command_found);

    return 0;
}