#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

int main()
{
    int socketfd;
    int temp;
    int curr = -1;
    int data[5];

    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket Creation Failed.\n");
        exit(-1);
    }

    struct sockaddr_in serveraddress, clientaddress;

    memset(&serveraddress, 0, sizeof(serveraddress));
    memset(&clientaddress, 0, sizeof(clientaddress));

    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketfd, (const struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0)
    {
        perror("Bind Failed");
        exit(-1);
    }

    while (1)
    {

        int n, len = sizeof(clientaddress);
        n = recvfrom(socketfd, &temp, sizeof(temp), 0, (struct sockaddr *)&clientaddress, &len);
        if(curr == 4) {
            char *message = "Data Full\n";
            sendto(socketfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&clientaddress, sizeof(clientaddress));
            close(socketfd);
        }
        if (curr == -1)
        {
            data[++curr] = temp;
            char *message = "Data stored\n";
            sendto(socketfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&clientaddress, sizeof(clientaddress));
        }
        else if (temp == data[curr])
        {
            char *message = "Duplicated, please send next data\n";
            sendto(socketfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&clientaddress, sizeof(clientaddress));                                                                                                                                                                                                                                                                                                                                                                                                                                                        
        }
        else if (temp == curr + 2)
        {
            data[++curr] = temp;
            char *message = "Data stored\n";
            sendto(socketfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&clientaddress, sizeof(clientaddress));
        }
        else
        {
            char *message = "Data Lost, please send correct data\n";
            sendto(socketfd, (const char *)message, strlen(message), 0, (const struct sockaddr *)&clientaddress, sizeof(clientaddress));
        }
    }

    close(socketfd);
    return 0;
}