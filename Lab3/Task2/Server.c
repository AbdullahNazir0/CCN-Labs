#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT       8080
#define MAXLINE    1024

int main() {
    int socketFd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";

    struct sockaddr_in serverAddr, clientAddr;
    if((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
		exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(socketFd, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
		exit(EXIT_FAILURE);
    }

    listen(socketFd, 5);
    int newFd;

    while(1) {
        int len = sizeof(clientAddr), n;
        newFd = accept(socketFd,(struct sockaddr*) &clientAddr, &len);

        n = recv(newFd, (char *)buffer, MAXLINE, 0);
        if(n < 0) {
            perror("Receive failed\n");
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';

        printf("Encrypted message recieved from client: %s\n", buffer);

        // decryption.
        for(int i = 0; i < strlen(buffer); i++) {
            // if(buffer[i] + 3 >= 'a' && buffer[i] + 3 <= 'z') {
            //     buffer[i] -= 3;
            // } else if(buffer[i] + 2 >= 'A' && buffer[i] + 2 <= 'Z') {
            //     buffer[i] -= 2;
            // } else if(buffer[i] + 1 >= '0' && buffer[i] + 1 <= '9') {
            //     buffer[i] -= 1;
            // }
            if(buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] = ((buffer[i] - 'a' - 3 + 26) % 26) + 'a';
            } else if(buffer[i] >= 'A' && buffer[i] <= 'Z') {
                buffer[i] = ((buffer[i] - 'A' - 2 + 26) % 26) + 'A';
            } else if(buffer[i] >= '0' && buffer[i] <= '9') {
                buffer[i] = ((buffer[i] - '0' - 1 + 10) % 10) + '0';
            }
        }

        send(newFd, (const char *) buffer, strlen(buffer), 0);
        printf("Decrypted message send to client.\n");
        close(newFd);
    }
    return 0;
}
