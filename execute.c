#include "execute.h"
#include "helper.h"

void execute(char *path, char **args)
{
    pid_t pid;
    pid = fork();
    int status;

    if (pid < 0){
        // TODO : error handler
        perror("Fork Failed: ");
    } else if (pid == 0){
        execvp(path, args);
        perror("In exec(): ");
    } else{
        pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("The process ended with exit(%d).\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("The process ended with kill -%d.\n", WTERMSIG(status));
        }
    }
}

void bello(){
    execute( "whoami", (char *[]){"whoami",NULL});
    execute( "hostname", (char *[]){"hostname",NULL});
    // Last executed command
    execute( "tty", (char *[]){"tty",NULL});
    char shell_command[] = "echo $SHELL";
    echo(shell_command, 12);
    execute( "pwd", (char *[]){"pwd",NULL});
    execute( "date", (char *[]){"date",NULL});
    // current number of processes being executed
}


char* string_in_reverse(char *input){
    int length = strlen(input);
    char *inverse = malloc(length);
    for (int i = 0; i < length; i++){
        inverse[i] = input[length - i - 1];
    }
    return inverse;
}

void write_to_file(char *input, int index, int mode, int reversed){
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

    if (reversed){
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

// TODO : !!! TTY & empty string & free memory strdups 
void echo(char *input, int length){
    char *check = malloc(length);
   // char *env = malloc(length);
    int check_index;

    // triple redirect 
    check = strstr(input, ">>>");
    check_index = check - input;
    if ((check_index < length ) && (check_index > 0)) {
        write_to_file(input, check_index, 1, 1);
        return;
    }
   
    // double redirect
    check = strstr(input, ">>");
    check_index = check - input;
    if ((check_index < length ) && (check_index > 0)) {
        write_to_file(input, check_index, 1, 0);
        return;
    }
    
    // single redirect
    check = strstr(input, ">");
    check_index = check - input;
    if ((check_index < length ) && (check_index > 0)) {
        write_to_file(input, check_index, 0, 0);
        return;
    }
    
    
    // <<<<<<<<<<<<<<<<<<<<<<<< variables -------------------------
    // assume there is only 1 variable and no quotes
    char *dollar = strstr(input, "$");
    int dollar_index = dollar - input;
    if ((dollar_index < length ) && (dollar_index > 0)) {
        // initialize variables
        char *env = malloc(length - dollar_index - 1);
        // parse input
        char *token = strtok(input, " ");
        int arg_count = 0;
        char *args[20];
        while (token != NULL) {
            token = trim(token);
            if (strlen(token) != 0) {
                if (token[0] == '$'){
                    strncpy(env, token + 1, strlen(token) - 1);
                    env[strlen(token) - 1] = '\0';
                    env = getenv(env);
                    args[arg_count++] = env;
                }else{
                    args[arg_count++] = token;
                }
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        execute( "echo", args);

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
    execute( "echo", (char *[]){"echo", write_this, NULL});
    // --------------------- other echo commands >>>>>>>>>>>>>>>>>>>>>>
    
    // free memory
    free(check);
    free(write_this);

}
