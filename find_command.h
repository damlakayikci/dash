#include <dirent.h> // opendir, readdir, closedir. DIR
#include <sys/stat.h> // stat, struct stat

extern int *command_found;   

void trimNewline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}


void findFilesRecursively(char *baseDirectory, char *file) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(baseDirectory);

    trimNewline(file);


    // Unable to open directory stream
    if (!dir) {
       // printf("Unable to open directory stream\n");
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
                    // printf("File: %s\n", path);
                    *command_found = 1;
                    return;
                }
              // printf("File: %s\n", path);
            } else if (S_ISDIR(path_stat.st_mode)) {
              // printf("Directory: %s\n", path);
                findFilesRecursively(path, file);
            }
        }
    }

    closedir(dir);
}
