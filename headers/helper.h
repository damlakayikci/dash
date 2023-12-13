#ifndef HELPER_H
#define HELPER_H

/**
 * @file helper.h
 * @brief Helper function declarations for the project.
 * @author Damla Kayikci
 * @date 09/12/2023
 */

#include <ctype.h>    // needed for isspace()
#include <dirent.h>   // opendir, readdir, closedir. DIR
#include <pwd.h>      // needed for getpwuid()
#include <stdio.h>    // needed for printf()
#include <stdlib.h>   // needed for malloc(), free()
#include <string.h>   // needed for strlen(), strtok()
#include <sys/stat.h> // stat, struct stat
#include <unistd.h>   // needed for fork(), exec()

// Global variables
extern int *command_found;
extern char *found_path;

// Function declarations
void print_command_prompt();
/*
    Prints the command prompt in the format:
    "username@hostname:current_directory --- "
*/

void findFilesRecursively(char *baseDirectory, char *file);
/**
    Finds the given file in the given directory and its subdirectories recursively.
    @param baseDirectory: The directory to start searching from.
    @param file: The file to search for.
*/

void findCommandInPath(char *command, char *paths[], int path_count);
/**
    Using findFilesRecursively(), searches for the given command in the given paths.
    @param command: The command to search for.
    @param paths: The list of paths to search in.
    @param path_count: The number of paths in the list.
*/

char *trim(char *s);
/**
    Trims the leading and trailing whitespaces from the given string.
*/

#endif