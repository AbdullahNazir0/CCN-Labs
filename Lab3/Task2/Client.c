#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#define PORT       8080
#define MAXLINE    1024

int main() {
    char buffer[MAXLINE];
    FILE *filePtr = fopen("fileData.txt", "r");
    if(!filePtr) {
        perror("Error reading file data\n");
        exit(EXIT_FAILURE);
    }
    char encryptedMessage[MAXLINE];
    fgets(encryptedMessage, MAXLINE, filePtr);
    fclose(filePtr);

    // encryption.
    encryptedMessage[strlen(encryptedMessage) - 1] = '\0'; // remove new line(last character)
    for(int i = 0; i < strlen(encryptedMessage); i++) {
        // if(encryptedMessage[i] >= 'a' && encryptedMessage[i] <= 'z') {
        //     encryptedMessage[i] += 3;
        // } else if(encryptedMessage[i] >= 'A' && encryptedMessage[i] <= 'Z') {
        //     encryptedMessage[i] += 2;
        // } else if(encryptedMessage[i] >= 0 && encryptedMessage[i] <= 9) {
        //     encryptedMessage[i] += 1;
        // }
        if(encryptedMessage[i] >= 'a' && encryptedMessage[i] <= 'z') {
            encryptedMessage[i] = ((encryptedMessage[i] - 'a' + 3) % 26) + 'a';
        } else if(encryptedMessage[i] >= 'A' && encryptedMessage[i] <= 'Z') {
            encryptedMessage[i] = ((encryptedMessage[i] - 'A' + 2) % 26) + 'A';
        } else if(encryptedMessage[i] >= '0' && encryptedMessage[i] <= '9') {
            encryptedMessage[i] = ((encryptedMessage[i] - '0' + 1) % 10) + '0';
        }
    }

    int socketFd;
    struct sockaddr_in serverAddr;
    if((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
		exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(connect(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Failed to open connection.\n");
        exit(EXIT_FAILURE);
    }
    int n, len;

    send(socketFd, (const char *)encryptedMessage, strlen(encryptedMessage), 0);
    printf("Encrypted message send to server\n");

    n = recv(socketFd, (char *)buffer, MAXLINE, 0);
    if(n < 0) {
        perror("Receive failed\n");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\n';
    printf("Decrypted Message recieved from server: %s\n", buffer);

    close(socketFd);
    return 0;
}
