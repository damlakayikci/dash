#include "helper.h"


int *command_found; 
char *found_path;


void findFilesRecursively(char *baseDirectory, char *file) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(baseDirectory);

    file = trim(file);

    // printf("Searching for %s in %s\n", file, baseDirectory);
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
                    strcpy(found_path, path);
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

char *ltrim(char *s){
    while(isspace(*s)) 
        s++;
    return s;
}

char *rtrim(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s){
    return rtrim(ltrim(s)); 
}