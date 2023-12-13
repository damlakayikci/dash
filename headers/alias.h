#ifndef ALIAS_H
#define ALIAS_H

/**
 * @file alias.h
 * @brief Declarations of the alias functions for the project.
 * @date 09/12/2023
 */

#include "helper.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *shortcut;
    char *command;
} Alias;

typedef struct {
    Alias *array;
    size_t used;
    size_t size;
} AliasArray;

int find_alias(AliasArray *a, char *shortcut);
/**
   Finds the index of the given alias in the given alias array.
   @param a: The alias array to search in.
   @param shortcut: The shortcut of the alias to search for.
   @return The index of the alias if found, -1 otherwise.
 */

void insertArray(AliasArray *a, char *shortcut, char *command);
/**
   Inserts the given alias to the given alias array.
   @param a: The alias array to insert to.
   @param shortcut: The shortcut of the alias to insert.
   @param command: The command of the alias to insert.
 */

void initArray(AliasArray *a, size_t initialSize);
/**
   Initializes the given alias array by reading from the aliases.txt file if it exists.
   @param a: The alias array to initialize.
   @param initialSize: The initial size of the alias array.
 */

void freeArray(AliasArray *a);
/**
   Frees the given alias array before exit.
   @param a: The alias array to free.
 */

void check_alias(AliasArray *a, char *input, int length);
/**
   Checks if the given input is an alias and inserts it to the list if it is.
   @param a: The alias array to search in.
   @param input: The input to check.
   @param length: The length of the input.
 */

#endif
