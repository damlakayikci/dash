
#include "trim.h"

int main(){
   
    char input[1024];
    fgets(input, sizeof(input), stdin);
    
    char *token = input;
    printf("Input before is %s\n", token);
    printf("Length before is %lu\t\t\t\n", strlen(token));
    token = trim(token);
    printf("Input after is %s\n", token);
    printf("Length after is %lu\t\t\t\n", strlen(token));
 
    return 0;


}