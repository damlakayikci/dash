#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    const char* hostname = "example.com";
    struct hostent* host_info = gethostbyname(hostname);

    if (host_info == NULL) {
        herror("gethostbyname");
        return 1;
    }

    // Corrected format specifier and argument
    printf("Official Name: %s\n", host_info->h_name);

    // Print alias names (if available)
    if (host_info->h_aliases[0] != NULL) {
        printf("Alias Names:\n");
        for (int i = 0; host_info->h_aliases[i] != NULL; i++) {
            printf("  %s\n", host_info->h_aliases[i]);
        }
    }

    // Print IP addresses (if available)
    if (host_info->h_addr_list[0] != NULL) {
        printf("IP Addresses:\n");
        for (int i = 0; host_info->h_addr_list[i] != NULL; i++) {
            struct in_addr addr;
            memcpy(&addr, host_info->h_addr_list[i], sizeof(struct in_addr));
            printf("  %s\n", inet_ntoa(addr));
        }
    }

    return 0;
}
