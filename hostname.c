// C program to display hostname
// and IP address
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	char hostbuffer[2560];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;

	// To retrieve hostname
	                      
    struct hostent* host;
    host =gethostbyaddr(hostbuffer, sizeof(hostbuffer) , AF_INET);

    printf("%s", host->h_name);

	return 0;
}
