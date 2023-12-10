#ifndef ALIAS_H
#define ALIAS_H

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

int find_alias(AliasArray *a, const char *shortcut);

void insertArray(AliasArray *a, const char *shortcut, const char *command);

void initArray(AliasArray *a, size_t initialSize);

void freeArray(AliasArray *a);

void check_alias(AliasArray *a, char *input, int length);

#endif
