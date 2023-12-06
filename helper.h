#ifndef HELPER_H
#define HELPER_H

#include <stdio.h> // needed for printf()
#include <string.h> // needed for strlen()
#include <ctype.h> // needed for isspace()
#include <dirent.h> // opendir, readdir, closedir. DIR
#include <sys/stat.h> // stat, struct stat

// Global variables
extern int *command_found; 
extern char *found_path;

void findFilesRecursively(char *baseDirectory, char *file);


char *trim(char *s);

#endif