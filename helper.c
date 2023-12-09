#include "helper.h"

int *command_found;
char *found_path;

void print_command_prompt() {
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
    }
}

void findCommandInPath(char *command, char *paths[], int path_count) {
    for (int i = 0; i < path_count; i++) {
        findFilesRecursively(paths[i], command);
        if (*command_found) {
            // TODO : remove this
            printf("Command found in %s\n", paths[i]);
            printf("Command is %s\n", found_path);
            // TODO : remove this
            break;
        }
    }
}

void findFilesRecursively(char *baseDirectory, char *file) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(baseDirectory);

    file = trim(file);

    if (!dir) {
        fprintf(stderr, "Cannot open directory %s\n", baseDirectory);
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            // Construct new path from our base path
            strcpy(path, baseDirectory);
            strcat(path, "/");
            strcat(path, dp->d_name);

            struct stat path_stat;
            stat(path, &path_stat);
            if (S_ISREG(path_stat.st_mode)) {

                if (strcmp(dp->d_name, file) == 0) {
                    *command_found = 1;
                    strcpy(found_path, path);
                    return;
                }
            } else if (S_ISDIR(path_stat.st_mode)) {
                findFilesRecursively(path, file);
            }
        }
    }
    closedir(dir);
}

char *ltrim(char *s) {
    while (isspace(*s))
        s++;
    return s;
}

char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace(*--back))
        ;
    *(back + 1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s));
}