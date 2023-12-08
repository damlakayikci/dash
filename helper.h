#ifndef HELPER_H
#define HELPER_H

#include <stdio.h> // needed for printf()
#include <string.h> // needed for strlen(), strtok()
#include <ctype.h> // needed for isspace()
#include <dirent.h> // opendir, readdir, closedir. DIR
#include <sys/stat.h> // stat, struct stat
#include <pwd.h>    // needed for getpwuid()
#include <unistd.h> // needed for fork(), exec()

// Global variables
extern int *command_found; 
extern char *found_path;


void print_command_prompt();

void findFilesRecursively(char *baseDirectory, char *file);

void findCommandInPath(char *command, char *paths[], int path_count);

char *trim(char *s);

#endif