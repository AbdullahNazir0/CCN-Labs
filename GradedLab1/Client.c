#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_LINE 1024

int main() {
    int socketfd ;
    char buffer[MAX_LINE];

    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket Creation Failed.\n");
        exit(-1);
    }

    struct sockaddr_in serveraddress;

    memset(&serveraddress, 0, sizeof(serveraddress));

    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int n, len = sizeof(serveraddress);

    int num = 6;
    int *ptr = &num;
    sendto(socketfd, (const int *)ptr, sizeof(int), 0, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));

    n = recvfrom(socketfd, (char *)buffer, MAX_LINE, 0, (struct sockaddr *)&serveraddress, &len);
    buffer[n] = '\0';
    write(1, buffer, strlen(buffer));

    close(socketfd);
    return 0;
}