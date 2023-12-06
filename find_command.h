#include <dirent.h> // opendir, readdir, closedir. DIR
#include <sys/stat.h> // stat, struct stat

extern int *command_found; 
extern char *found_path;

void trimNewline(char *str);


void findFilesRecursively(char *baseDirectory, char *file);