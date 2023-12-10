#include "alias.h"

int find_alias(AliasArray *a, const char *shortcut) {
    if (a->used == 0) {
        return -1;
    }

    for (size_t i = 0; i < a->used; i++) {
        printf("Shortcut: command %s: %s\n", a->array[i].shortcut, a->array[i].command);
        if (strcmp(a->array[i].shortcut, shortcut) == 0) {
            return i;
        }
    }
    return -1;
}

void insertArray(AliasArray *a, const char *shortcut, const char *command) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(Alias));
    }
    // Check if shortcut already exists
    int index = find_alias(a, shortcut);
    if (index != -1) {
        printf("shortcut already exists\n");
        free(a->array[index].command);
        a->array[index].command = trim(strdup(command));
        return;
    }
    a->array[a->used].shortcut = strdup(shortcut);
    printf("command: %s\n", command);
    a->array[a->used].command = trim(strdup(command));
    a->used++;
}

void initArray(AliasArray *a, size_t initialSize) {
    a->array = malloc(initialSize * sizeof(Alias));
    a->used = 0;
    a->size = initialSize;

    // Read from file "alias.txt"
    FILE *file_pointer;
    char line[1024]; // Array to hold each line

    file_pointer = fopen("alias.txt", "r");
    if (file_pointer == NULL) {
        return;
    }
    printf("File exists\n");

    // Read each line and insert it into the array
    while (fgets(line, sizeof(line), file_pointer)) {
        if (line[0] == '\n') {
            continue;
        }
        printf("Line: %s", line);
        char *shortcut = strtok(line, "=");
        char *command = strtok(NULL, "=");
        printf("%s: %s\n", shortcut, command);

        insertArray(a, shortcut, command);
    }

    // Close the file
    fclose(file_pointer);
}

void freeArray(AliasArray *a) {
    // Write to file "alias.txt"
    FILE *file_pointer;
    file_pointer = fopen("alias.txt", "w");

    if (file_pointer == NULL) {
        fprintf(stderr, "Error saving alias\n");
    }
    for (size_t i = 0; i < a->used; i++) {
        printf("Write back to file %s: %s\n", a->array[i].shortcut, a->array[i].command);
        fprintf(file_pointer, "%s=%s\n", a->array[i].shortcut, a->array[i].command);
        free(a->array[i].shortcut);
        free(a->array[i].command);
    }
    fclose(file_pointer);
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void check_alias(AliasArray *a, char *input, int length) {
    // initialize variables
    char *alias_name = malloc(length);
    char *alias_command = malloc(length);
    char *token = strtok(input, "=");

    int arg_count = 0;

    while (token != NULL) {
        printf("token: %s\n", token);
        token = trim(token);
        printf("token: %s\n", token);
        if (arg_count > 1) {
            fprintf(stderr, "alias: too many arguments\n");
            free(alias_name);
            free(alias_command);
            return;
        }
        if (strlen(token) != 0) {
            if (arg_count == 0) { // first argument is the alias name
                strncpy(alias_name, token + 6, strlen(token));
                char *trimmed = trim(alias_name);
                strncpy(alias_name, trimmed, strlen(trimmed));
                alias_name[strlen(trimmed)] = '\0';
                if (strlen(alias_name) == 0) {
                    fprintf(stderr, "alias: invalid alias name\n");
                    free(alias_name);
                    free(alias_command);
                    return;
                }
            } else {
                strncpy(alias_command, token, strlen(token));
                char *trimmed = trim(alias_command);
                strncpy(alias_command, trimmed, strlen(trimmed));
                alias_command[strlen(trimmed)] = '\0';
                // if the command doesnt start and end with quotes, it is invalid
                if (((alias_command[0] != '"') && (alias_command[0] != '\'')) || ((alias_command[strlen(alias_command) - 1] != '"') && (alias_command[strlen(alias_command) - 1] != '\''))) {
                    fprintf(stderr, "alias: invalid command format\n");
                    free(alias_name);
                    free(alias_command);
                    return;
                }
                // trim the quotes
                strncpy(alias_command, alias_command + 1, strlen(alias_command) - 2);
                alias_command[strlen(alias_command) - 2] = '\0';
            }
            arg_count++;
        }
        token = strtok(NULL, "=");
    }

    printf("alias_name: %s\n", alias_name);
    printf("alias_command: %s\n", alias_command);
    insertArray(a, alias_name, alias_command);

    // free memory
    free(alias_name);
    free(alias_command);
}

// int main() {
//     AliasArray a;
//     initArray(&a, 5); // Initialize array with an initial size of 5

//     // get input
//     char input[1024];
//     fgets(input, sizeof(input), stdin);

//     // trim input
//     char *token = input;
//     token = strtok(token, "=");
//     token = trim(token);
//     char *shortcut = token;
//     token = strtok(NULL, "=");
//     token = trim(token);
//     char *command = token;

//     // insert into array
//     insertArray(&a, shortcut, command);
//     // insertArray(&a, "ls", "list");

//     // insertArray(&a, "pwd", "liasdasst");
//     // insertArray(&a, "rm", "remove");

//     // Example: Print the aliases
//     for (size_t i = 0; i < a.used; i++) {
//         printf("%s: %s\n", a.array[i].shortcut, a.array[i].command);
//     }

//     freeArray(&a);
//     return 0;
// }
