#include "execute.h"

int execute(char *path, char **args)
{
    pid_t pid;
    pid = fork();
    int status;

    if (pid < 0){
        // TODO : error handler
        perror("Fork Failed: ");
        return 1;
    }
    else if (pid == 0){
        execv(path, args);
        perror("In exec(): ");
        //execvp(command, args);
    }
    else{
        pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("The process ended with exit(%d).\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("The process ended with kill -%d.\n", WTERMSIG(status));
        }
    }
}
