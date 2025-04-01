#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int socketFd;
    struct sockaddr_in serverAddr;
    char filename[MAXLINE], buffer[MAXLINE];

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\nEnter file name: ");
    fgets(filename, MAXLINE, stdin);
    // filename[strcspn(filename, "\n")] = 0;
    filename[strlen(filename) - 1] = '\0';

    send(socketFd, filename, strlen(filename), 0);

    FILE *filePtr = fopen("received_file.txt", "w");
    if (filePtr == NULL) {
        perror("Error in creating file");
        exit(EXIT_FAILURE);
    }

    int n;
    while ((n = recv(socketFd, buffer, MAXLINE, 0)) > 0) {
        fwrite(buffer, 1, n, filePtr);
    }

    printf("File received successfully. Saved as 'received_file.txt'\n");

    fclose(filePtr);
    close(socketFd);
    return 0;
}
