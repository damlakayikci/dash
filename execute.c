#include "execute.h"
#include "helper.h"

char *previous; // previous command
int current_processes;

void execute_redirect(char *path, char **args, int mode, char *file_name, int background) {
    int flags;
    if (mode == 1) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (mode == 2) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else if (mode == 3) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    int filedes = open(file_name, flags, 0644);
    if (filedes < 0) {
        perror("Error opening file: ");
        return;
    }
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        close(filedes);
        perror("Fork Failed: ");
    } else if (pid == 0) {
        dup2(filedes, STDOUT_FILENO);
        close(filedes);
        execvp(path, args);
        perror("In exec(): ");
        exit(1); // Exit child process if exec fails
    } else {
        // Parent process
        current_processes++; // Increment for the newly created child process
        if (!background) {
            close(filedes);
            int status;
            waitpid(pid, &status, 0); // Wait for child process to finish
            current_processes--;      // Decrement as the child process has ended
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) { 
                printf("%s: process exited with status %d\n", path, WEXITSTATUS(status));
            }
        }
    }

}

void execute(char *path, char **args) {
    // check if last character is &
    int background = 0;
    int redirect_mode = 0;
    int redirect_index = 0;
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            redirect_mode = 1;
            redirect_index = i;
        } else if (strcmp(args[i], ">>") == 0){
            redirect_mode = 2;
            redirect_index = i;
        } else if (strcmp(args[i], ">>>") == 0){
            redirect_mode = 3;
            redirect_index = i;
        }
        i++;
    }
    
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        background = 1;     // Last argument is "&"
        args[i - 1] = NULL; // Remove "&" from args
    }
    if (redirect_mode != 0) {
        args[redirect_index] = NULL;
        char *file_name = args[redirect_index + 1];
        args[redirect_index + 1] = NULL;
        execute_redirect(path, args, redirect_mode, file_name, background);
        return;
    }
    
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork Failed: ");
    } else if (pid == 0) {
        execvp(path, args);
        perror("In exec(): ");
        exit(1); // Exit child process if exec fails
    } else {
        // Parent process
        current_processes++; // Increment for the newly created child process
        if (!background) {
            int status;
            waitpid(pid, &status, 0); // Wait for child process to finish
            current_processes--;      // Decrement as the child process has ended
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) { 
                printf("%s: process exited with status %d\n", path, WEXITSTATUS(status));
            }
        }
    }
}

void bello() {
    current_processes++;                               // Increment for bello process
    execute("whoami", (char *[]){"whoami", NULL});     // 1 username
    execute("hostname", (char *[]){"hostname", NULL}); // 2 hostname
    // Last executed Command
    if (previous != NULL) {
        printf("%s\n", previous); // 3 last executed command
    }
    execute("tty", (char *[]){"tty", NULL}); // 4 tty
    char shell_command[] = "echo $SHELL";
    echo(shell_command, 12);                                        // 5 current shell name
    execute("pwd", (char *[]){"pwd", NULL});                        // 6 current working directory
    execute("date", (char *[]){"date", NULL});                      // 7  date
    printf("Current number of processes: %d\n", current_processes); // 8 current number of processes being executed
}

char *string_in_reverse(char *input) {
    int length = strlen(input);
    char *inverse = malloc(length);
    for (int i = 0; i < length; i++) {
        inverse[i] = input[length - i - 1];
    }
    return inverse;
}

void write_to_file(char *input, int index, int mode, int reversed) {
    // initialize variables
    int input_length = strlen(input);
    int text_length = index - 5;
    char *text = malloc(index);
    char *file_name = malloc(input_length);
    FILE *file_pointer;
    const char *modes[2] = {"w\0", "a\0"};

    // get text & null terminate
    strncpy(text, input + 5, text_length);
    printf("text: %s\n", text);
    text[text_length] = '\0';
    printf("text: %s\n", text);

    if (reversed) {
        text = string_in_reverse(text);
        printf("text: %s\n", text);
        index = index + 3;
    } else {
        index = index + mode + 1;
    }
    // text = trim(text);

    // get file name & no need to null terminate since length is longer than available characters
    strncpy(file_name, input + index, input_length - index);
    printf("file_name: %s\n", file_name);

    file_pointer = fopen(file_name, modes[mode]);
    printf("file_pointer: %p\n", file_pointer);
    fprintf(file_pointer, "%s\n", text);
    fclose(file_pointer);

    // free memory
    free(text);
    free(file_name);
}

// TODO : !!! TTY & empty string
void echo(char *input, int length) {
    char *check = malloc(length);
    int check_index;

    // triple redirect
    check = strstr(input, ">>>");
    check_index = check - input;
    if ((check_index < length) && (check_index > 0)) {
        write_to_file(input, check_index, 1, 1);
        return;
    }

    // double redirect
    check = strstr(input, ">>");
    check_index = check - input;
    if ((check_index < length) && (check_index > 0)) {
        write_to_file(input, check_index, 1, 0);
        return;
    }

    // single redirect
    check = strstr(input, ">");
    check_index = check - input;
    if ((check_index < length) && (check_index > 0)) {
        write_to_file(input, check_index, 0, 0);
        return;
    }

    // -------------------- single redirect >>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<<<<<< variables -------------------------
    // assume there is only 1 variable and no quotes
    char *dollar = strstr(input, "$");
    int dollar_index = dollar - input;
    if ((dollar_index < length) && (dollar_index > 0)) {
        // initialize variables
        char *env = malloc(length - dollar_index - 1);
        // parse input
        char *token = strtok(input, " ");
        int arg_count = 0;
        char *args[20];
        while (token != NULL) {
            token = trim(token);
            if (strlen(token) != 0) {
                if (token[0] == '$') {
                    strncpy(env, token + 1, strlen(token) - 1);
                    env[strlen(token) - 1] = '\0';
                    char *env_value = getenv(env);
                    args[arg_count++] = env_value;
                } else {
                    args[arg_count++] = token;
                }
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        execute("echo", args);

        // free memory
        free(env);
        return;
    }
    // ------------------------- variables >>>>>>>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<< other echo commands ---------------------
    char *write_this = malloc(length - 5);
    strncpy(write_this, input + 5, length - 6);
    // null terminate
    write_this[length - 5] = '\0';
    execute("echo", (char *[]){"echo", write_this, NULL});
    // --------------------- other echo commands >>>>>>>>>>>>>>>>>>>>>>

    // free memory
    free(check);
    free(write_this);
}
