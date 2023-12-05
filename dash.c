#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()
#include <stdio.h> // needed for printf()
#include "find_command.h"

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

    // allocate memory
    command_found = malloc(sizeof(int));

    do{ 
        // initialize variables
        *command_found = 0;
        found_path = (char *) malloc(1024 * sizeof(char));

        // <<<<<<<<<<<<<<<<<< PATH variable ------------------
        char *original_path = strdup(getenv("PATH")); // !!! PATH burada farkli sirada geliyor
        // printf("PATH is %s\n", original_path);
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
        char *token = strtok(input, " ");
        arg_count = 0;
        while (token != NULL) {
            // printf("Token is %s\n", token);
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        // ---------------------- Input >>>>>>>>>>>>>>>>>>>>>>

        
        // traverse through paths
        for (int i = 0; i < path_count; i++) {
            // find command in path
            // printf("Searching in %s\n", paths[i]);
            findFilesRecursively(paths[i], args[0]);
            if (*command_found) {
                printf("Command found in %s\n", paths[i]);
                printf("Command is %s\n", found_path);
                break;
            }
        }

        // free memory
        free(found_path);
        free(original_path);

    } while(strcmp(args[0], "exit"));
    free(command_found);

    return 0;
}