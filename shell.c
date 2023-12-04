#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


int main(){

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
   
    uid_t uid = geteuid(); // 501
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        // Access and print the username field
        printf("Username: %s\n", pw->pw_name);
    } else {
        fprintf(stderr, "Error retrieving password entry.\n");
    }
    
    printf("%s@%s %s --- ", pw->pw_name, hostname, cwd);

    return 0;
}