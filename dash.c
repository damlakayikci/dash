#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()
#include <stdio.h> // needed for printf()
#include <unistd.h> // needed for fork(), exec()
#include <pwd.h>    // needed for getpwuid()
#include "helper.h"
#include "execute.h"


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
        // -------------------------------------- PATH variable >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< user info ------------------------------------------
        // Get current working directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        
        // Get hostname
        char hostname[1024];
        gethostname(hostname, sizeof(hostname));

        // Get username
        uid_t uid = geteuid(); // 501
        struct passwd *pw = getpwuid(uid);

        if (pw) {
        // Access and print the username field
            printf("%s@%s %s --- ", pw->pw_name, hostname, cwd);
        } else {
            // TODO : error handler
            fprintf(stderr, "Error retrieving password entry.\n");
            continue;
        } 
        // ------------------------------------------ user info >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Input ------------------------------------------
        // Get input script
        char input[1024];
        fgets(input, sizeof(input), stdin);

        // trim input
        char *token = input;
        token = trim(token);
        printf("Trimmed input is %s\n", token);
      
        // if token length is 0, there is no input. Ignore the rest
        if (strlen(token) == 0) {
            // TODO : remove this
            printf("________________ No input\n");
            // TODO : remove this
            continue;
        }

        // tokenize input
        token = strtok(token, " ");
        arg_count = 0;

        // get the first token as command and check if it is exit
        command = token;
        command = trim(command);
        comparison = strcmp(command, "exit"); 


        // if command is not exit
        if (comparison) {
            while (token != NULL) {
                // trim token
                token = trim(token);

                // if length of token is 0, ignore it
                if (strlen(token) != 0) {
                    args[arg_count++] = token;
                }

                // get next token
                token = strtok(NULL, " ");
            }

            // null terminate args
            args[arg_count] = NULL;

            // TODO : remove this
            printf("------------------------------ HERE ARE THE ARGUMENTS ------------------------------\n");
            printf("Arg count is %d\n", arg_count);
            for (int i = 0; i < arg_count; i++) {
                printf("Token %d is %s\n", i, args[i]);
               // printf("Length of token %d is %lu\n", i, strlen(args[i]));
            }
            // TODO : remove this


            // ------------------------------------------ Input >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
               
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
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Execute ------------------------------------------
            if (*command_found) {
            
                // --------------------- cd ---------------------
                if (strcmp(command, "cd") == 0) {
                    
                    if (arg_count > 2){
                        // TODO : error handler
                        fprintf(stderr, "cd: too many arguments\n");
                    }
                    else if (arg_count == 2){
                        int result = chdir(args[1]);
                        if (result == -1){
                            // TODO : error handler
                            fprintf(stderr, "cd: no such file or directory: %s\n", args[1]);
                        }
                    }
                    else{
                        char *home = getenv("HOME");
                        int result = chdir(home);
                        if (result == -1){
                            // TODO : error handler
                            fprintf(stderr, "cannot get environment variable HOME\n");
                        }
                    }
                    continue;
                }
                // --------------------- cd ---------------------

                execute(found_path, args);
                // pid_t pid;
                // pid = fork();

                // if (pid < 0){
                //     // TODO : error handler
                //     fprintf(stderr, "Fork Failed");
                //     return 1;
                // }
                // else if (pid == 0){
                // execv(found_path, args);
                // printf("Command not found\n");
                // continue;
                // //execvp(command, args);
                // }
                // else{
                //     wait(NULL);
                // }
            // ------------------------------------------ Execute >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            }
        }
        
        // free memory
        free(found_path);
        free(original_path);

    } while(comparison);

    free(command_found);

    return 0;
}