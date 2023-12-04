#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>


int main(){
    char *args[40];

    do {

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
            fprintf(stderr, "Error retrieving password entry.\n");
            continue;
        }
    
        // Get input script
        char input[1024];
        fgets(input, sizeof(input), stdin);
        // parse input
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        

    } while (strcmp(args[0], "exit\n"));

    
   
    
    

    return 0;
}