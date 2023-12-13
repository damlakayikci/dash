#include "execute.h"
#include "helper.h"

int current_processes;

void execute_redirect(char *path, char **args, int mode, char *file_name, int background) {
    int flags, p[2], nbytes;
    char inbuf[1023]; // initialize to max string size of 1023

    if (pipe(p) < 0) {
        perror("pipe"); // if pipe fails
        return;
    }

    if (mode == 1) {
        flags = O_WRONLY | O_CREAT | O_TRUNC; // w mode
    } else if (mode == 2) {
        flags = O_WRONLY | O_CREAT | O_APPEND; // a mode
    } else if (mode == 3) {
        flags = O_WRONLY | O_CREAT | O_APPEND; // a mode
    }

    int filedes = open(file_name, flags, 0644);
    if (filedes < 0) {
        perror("Error opening file: ");
        return;
    }
    pid_t pid, g_pid; // g_pid is the pid of the grandchild process
    pid = fork();

    if (pid < 0) {
        close(filedes);
        perror("Fork Failed: ");
    } else if (pid == 0) { // Child process
        g_pid = fork();

        if (g_pid < 0) {
            perror("forking grandchild");
            exit(1);
        } else if (g_pid == 0) { // Grandchild process
            // Redirect stdout to the write end of the pipe
            dup2(p[1], STDOUT_FILENO);
            close(p[0]); // Close the read end of the pipe in the grandchild
            close(p[1]); // Close the write end of the pipe

            // Execute the command
            execvp(path, args);
            perror("execvp");
            exit(1);

        } else {         // Child process
            close(p[1]); // Close the write end of the pipe in the child

            // Wait for the grandchild to finish
            wait(NULL);

            // Read from the pipe and write to the file
            nbytes = read(p[0], inbuf, 1023);
            if (nbytes > 0) {         // nbytes is going to give us the number of bytes read
                inbuf[nbytes] = '\0'; // Null-terminate the string
            }
            close(p[0]); // Close the read end of the pipe in the parent
            char *out = trim(inbuf);

            if (mode == 3) {
                write(filedes, string_in_reverse(out), nbytes - 1);
            } else {
                write(filedes, out, nbytes - 1);
            }

            close(filedes);
            exit(0);
        }

    } else {
        // Parent process
        if (!background) {
            int status;
            waitpid(pid, &status, 0); // Wait for child process to finish
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
        } else if (strcmp(args[i], ">>") == 0) {
            redirect_mode = 2;
            redirect_index = i;
        } else if (strcmp(args[i], ">>>") == 0) {
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

int bello(char **args, char *previous, int arg_count) {
    if (arg_count > 4) {
        fprintf(stderr, "bello: too many arguments\n");
        return 1;
    }
    current_processes++; // Increment for bello process
    int char_count = 0;  // Number of characters written to buffer
    char buffer[1024];

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char *tty_name = ttyname(STDIN_FILENO);
    char *shell = getenv("SHELL");
    // Time
    time_t now;
    struct tm *tm_info;
    char time_buffer[80];
    time(&now);                                                 // Get current time
    tm_info = localtime(&now);                                  // Convert to local time format
    strftime(time_buffer, 80, "%a %b %d %H:%M:%S %Y", tm_info); // Format time, "Day Month Date HH:MM:SS YYYY"

    // Print to file
    char_count = sprintf(buffer, "%s\n", pw->pw_name);
    char_count += sprintf(buffer + char_count, "%s\n", hostname);
    char_count += sprintf(buffer + char_count, "%s\n", previous);
    char_count += sprintf(buffer + char_count, "%s\n", tty_name);
    char_count += sprintf(buffer + char_count, "%s\n", shell);
    char_count += sprintf(buffer + char_count, "%s\n", cwd);
    char_count += sprintf(buffer + char_count, "%s\n", time_buffer);
    char_count += sprintf(buffer + char_count, "%d\n", current_processes);
    buffer[char_count] = '\0';

    // Shift elements of args to the right
    for (int i = arg_count; i > 0; i--) {
        args[i] = args[i - 1];
    }
    args[0] = "echo";
    args[1] = buffer;

    // print elements of args
    for (int i = 0; i < arg_count + 1; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }

    execute("echo", args);
    current_processes--; // Decrement as the bello process has ended
    return 0;
}

char *string_in_reverse(char *input) {
    int length = strlen(input);
    char *inverse = malloc(length);
    for (int i = 0; i < length; i++) {
        inverse[i] = input[length - i - 1];
    }
    return inverse;
}

void echo(char *input, int length) {
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
                    if (env_value != NULL) {
                        args[arg_count++] = env_value;
                    } else {
                        args[arg_count++] = token;
                    }
                } else {
                    args[arg_count++] = token;
                }
            }
            token = strtok(NULL, " ");
        }
        for (int i = 0; i < arg_count; i++) {
            printf("args[%d]: %s\n", i, args[i]);
        }
        args[arg_count] = NULL;
        execute("echo", args);

        // free memory
        free(env);
        return;
    }
    // ------------------------- variables >>>>>>>>>>>>>>>>>>>>>>>>>>>

    char *check;
    int check_index;
    char *write_this = malloc(length - 5);
    char *args[6];
    args[0] = "echo";
    // triple redirect
    check = strstr(input, ">>>");
    check_index = check - input;
    if ((check_index < length - 3) && (check_index > 0)) {
        strncpy(write_this, input + 5, check_index - 6);
        char *file = malloc(length - check_index - 5);
        strncpy(file, input + check_index + 3, length);
        char *file_name = trim(file);
        // if last char is &, remove it
        if (file_name[strlen(file_name) - 1] == '&' && file_name[strlen(file_name) - 2] == ' ') {
            file_name[strlen(file_name) - 2] = '\0';
            args[4] = "&";
            args[5] = NULL;
        } else {
            args[4] = NULL;
        }
        args[1] = write_this;
        args[2] = ">>>";
        args[3] = file_name;
        execute("echo", args);
        // free memory
        free(write_this);
        free(file);
        return;
    }

    // double redirect
    check = strstr(input, ">>");
    check_index = check - input;
    if ((check_index < length - 2) && (check_index > 0)) {
        strncpy(write_this, input + 5, check_index - 6);
        char *file = malloc(length - check_index - 5);
        strncpy(file, input + check_index + 2, length);
        char *file_name = trim(file);
        // if last char is &, remove it
        if (file_name[strlen(file_name) - 1] == '&' && file_name[strlen(file_name) - 2] == ' ') {
            file_name[strlen(file_name) - 2] = '\0';
            args[4] = "&";
            args[5] = NULL;
        } else {
            args[4] = NULL;
        }
        args[1] = write_this;
        args[2] = ">>";
        args[3] = file_name;
        args[4] = NULL;
        execute("echo", args);
        // free memory
        free(write_this);
        free(file);
        return;
    }

    // single redirect
    check = strstr(input, ">");
    check_index = check - input;
    if ((check_index < length - 1) && (check_index > 0)) {
        strncpy(write_this, input + 5, check_index - 6);
        printf("write_this: %s\n", write_this);
        char *file = malloc(length - check_index - 5);
        strncpy(file, input + check_index + 1, length);
        printf("file: %s\n", file);
        char *file_name = trim(file);
        printf("file_name: %s\n", file_name);
        // if last char is &, remove it
        if (file_name[strlen(file_name) - 1] == '&' && file_name[strlen(file_name) - 2] == ' ') {
            file_name[strlen(file_name) - 2] = '\0';
            args[4] = "&";
            args[5] = NULL;
        } else {
            args[4] = NULL;
        }
        args[1] = write_this;
        args[2] = ">";
        args[3] = file_name;
        args[4] = NULL;
        execute("echo", args);
        // free memory
        free(write_this);
        free(file);
        return;
    }

    strncpy(write_this, input + 5, length);
    printf("write_this: %s\n", write_this);
    // null terminate
    write_this[length - 5] = '\0';
    args[1] = write_this;
    args[2] = NULL;

    execute("echo", args);

    // free memory

    free(write_this);
}
