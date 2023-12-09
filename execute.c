#include "execute.h"
#include "helper.h"

void execute(char *path, char **args) {
    pid_t pid;
    pid = fork();
    int status;

    if (pid < 0) {
        // TODO : error handler
        perror("Fork Failed: ");
    } else if (pid == 0) {
        execvp(path, args);
        perror("In exec(): ");
    } else {
        pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("The process ended with exit(%d).\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("The process ended with kill -%d.\n", WTERMSIG(status));
        }
    }
}

void bello() {
    execute("whoami", (char *[]){"whoami", NULL});
    execute("hostname", (char *[]){"hostname", NULL});
    // Last executed command
    execute("tty", (char *[]){"tty", NULL});
    char shell_command[] = "echo $SHELL";
    echo(shell_command, 12);
    execute("pwd", (char *[]){"pwd", NULL});
    execute("date", (char *[]){"date", NULL});
    // current number of processes being executed
}

char *string_in_reverse(char *input) {
    int length = strlen(input);
    char *inverse = malloc(length);
    for (int i = 0; i < length; i++) {
        inverse[i] = input[length - i - 1];
    }
    return inverse;
}

// TODO : !!! TTY & empty string & free memory strdups
void echo(char *input, int length) {
    // <<<<<<<<<<<<<<<<<<<< triple redirect ---------------------
    char *triple_redirect = strstr(input, ">>>");
    int triple_redirect_index = triple_redirect - input;
    // printf("triple_redirect_index: %d\n", triple_redirect_index);
    if ((triple_redirect_index < length) && (triple_redirect_index > 0)) {
        char *write_this = malloc(triple_redirect_index - 4);
        char *reversed_string = malloc(triple_redirect_index - 4);
        char *append_to = malloc(length - triple_redirect_index - 1);
        FILE *fptr;
        strncpy(write_this, input + 5, triple_redirect_index - 5);
        // null terminate
        write_this[triple_redirect_index - 5] = '\0';
        reversed_string = string_in_reverse(write_this);
        strncpy(append_to, input + triple_redirect_index + 3, length - triple_redirect_index - 3);
        // null terminate
        append_to[length - triple_redirect_index - 3] = '\0';
        fptr = fopen(append_to, "a");
        fprintf(fptr, "%s\n", reversed_string);
        fclose(fptr);
        return;
    }
    // --------------------- triple redirect >>>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<< double redirect ---------------------
    char *double_redirect = strstr(input, ">>");
    int double_redirect_index = double_redirect - input;
    // printf("double_redirect_index: %d\n", double_redirect_index);
    if ((double_redirect_index < length) && (double_redirect_index > 0)) {
        // initialize variables
        FILE *fptr;
        char *write_this = malloc(double_redirect_index - 4);
        char *append_to = malloc(length - double_redirect_index - 1);

        strncpy(write_this, input + 5, double_redirect_index - 5);
        // null terminate
        write_this[double_redirect_index - 5] = '\0';

        strncpy(append_to, input + double_redirect_index + 2, length - double_redirect_index - 2);
        // null terminate
        append_to[length - double_redirect_index - 2] = '\0';
        printf("append_to: %s\n", append_to);

        // Append to file
        fptr = fopen(append_to, "a");
        fprintf(fptr, "%s\n", write_this);
        fclose(fptr);

        return;
    }
    // -------------------- double redirect >>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<< single redirect ---------------------
    char *single_redirect = strstr(input, ">");
    int single_redirect_index = single_redirect - input;
    // printf("single_redirect_index: %d\n", single_redirect_index);
    if ((single_redirect_index < length) && (single_redirect_index > 0)) {
        // initialize variables
        FILE *fptr;
        char *write_this = malloc(single_redirect_index - 4);
        char *write_to = malloc(length - single_redirect_index - 1);

        strncpy(write_this, input + 5, single_redirect_index - 5);
        // null terminate
        write_this[single_redirect_index - 5] = '\0';

        strncpy(write_to, input + single_redirect_index + 2, length - single_redirect_index - 2);
        // null terminate
        write_to[length - single_redirect_index - 2] = '\0';

        // Write to file
        fptr = fopen(write_to, "w");
        fprintf(fptr, "%s\n", write_this);
        fclose(fptr);
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
                    env = getenv(env);
                    args[arg_count++] = env;
                } else {
                    args[arg_count++] = token;
                }
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        execute("echo", args);

        return;
    }
    // ------------------------- variables >>>>>>>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<< other echo commands ---------------------
    char *write_this = malloc(length - 5);
    strncpy(write_this, input + 5, length - 5);
    // null terminate
    write_this[length - 5] = '\0';
    execute("echo", (char *[]){"echo", write_this, NULL});
    // --------------------- other echo commands >>>>>>>>>>>>>>>>>>>>>>
}
