// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	int rollNoToRecieve = 0x0788;
	int rollNoRecieved;
	char* name;
	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len=sizeof(cliaddr);
	int n = recvfrom(sockfd, (void *)&rollNoRecieved, sizeof(int), 0, ( struct sockaddr *) &cliaddr, &len);
	rollNoRecieved = ntohs(rollNoRecieved);
	printf("Received Roll No: 0x%04X\n", rollNoRecieved);
	if(rollNoRecieved == rollNoToRecieve) {
        name = "Abdullah Nazir";
	} else {
        name = "Roll no not found";
	}
	sendto(sockfd, (const char *)name, strlen(name), 0, (const struct sockaddr *) &cliaddr,sizeof(cliaddr));
	printf("Hello message sent.\n");

	return 0;
}
