#include <stdlib.h> // needed for getenv(), PATH
#include <string.h> // needed for strtok()



int main(){
    int path_count = 0;

    do{
        // Get PATH variable
        char* path = getenv("PATH");
        
        // Parse PATH
        char *path_token = strtok(path, ":");
        char *paths[40];
        
        while (path_token != NULL) {
            paths[path_count++] = path_token;
            path_token = strtok(NULL, ":");
        }

        // Get current working directory from "pwd" in /bin

        

        

    } while(1);


    return 0;
}