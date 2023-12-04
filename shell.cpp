#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>

  
using namespace std; 

int main(){

   // int input;
    cout << "egemenisguder@egemenPC " ;
    uid_t uid = geteuid();
    cout << uid << "\n";
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        // Access and print the username field
        cout << "Username: " << pw->pw_name << endl;
    } else {
        cerr << "Error retrieving password entry." << endl;
    }    
   // cin >> input;


    return 0;
}