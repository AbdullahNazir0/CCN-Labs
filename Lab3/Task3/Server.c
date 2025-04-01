#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAXLINE 1024

void send_file(FILE *filePtr, int clientFd) {
    char data[MAXLINE] = {0};

    while (fgets(data, MAXLINE, filePtr) != NULL) {
        if (send(clientFd, data, strlen(data), 0) == -1) {
            perror("Error in sending file.");
            exit(EXIT_FAILURE);
        }
        memset(data, 0, MAXLINE);
    }
}

int main() {
    int serverFd, clientFd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char filename[MAXLINE];

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        addr_size = sizeof(clientAddr);
        clientFd = accept(serverFd, (struct sockaddr *)&clientAddr, &addr_size);
        if (clientFd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: Port %d, Process ID: %d\n", ntohs(clientAddr.sin_port), getpid());

        recv(clientFd, filename, MAXLINE, 0);
        // filename[strcspn(filename, "\n")] = 0;
        filename[strlen(filename) - 1] = '\0';

        printf("Client requested file: %s\n", filename);

        FILE *filePtr = fopen(filename, "r");
        if (filePtr == NULL) {
            perror("File not found");
            send(clientFd, "File not found", 15, 0);
        } else {
            send_file(filePtr, clientFd);
            fclose(filePtr);
            printf("File sent successfully.\n");
        }

        close(clientFd);
        printf("Client disconnected. Server ready for new request.\n");
    }

    close(serverFd);
    return 0;
}
