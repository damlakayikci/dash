#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void listFilesRecursively(char *baseDirectory, char *file, int *found);

int main() {
    // Specify the directory path you want to start with
    char path[100];
    char file[100];
    int found = 0;

    // Input the directory path
    printf("Enter the directory path: ");
    scanf("%s", path);
    printf("Enter the file name: ");
    scanf("%s", file);

    listFilesRecursively(path, file, &found);
    printf("Found: %d\n", found);

    return 0;
}

void listFilesRecursively(char *baseDirectory, char *file, int *found) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(baseDirectory);

    // Unable to open directory stream
    if (!dir) 
        return;

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
                    printf("File: %s\n", path);
                    *found = 1;
                    return;
                }
               printf("File: %s\n", path);
            } else if (S_ISDIR(path_stat.st_mode)) {
               printf("Directory: %s\n", path);
                listFilesRecursively(path, file, found);
            }
        }
    }

    closedir(dir);
}
