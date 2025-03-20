#include <sys/socket.h>

int main() {
    // Create a socket
    // int socket(int family, int type, int protocol)
    int sid = socket(AF_INET, SOCK_DGRAM, 0);

    // Bind a socket
    // int bind(int sockfd, struct sockaddr* serverAddr, int addrlen)

    // Send data
    // int sendTo(int sockfd, char *buf, size_t nbytes, int flags, struct sockaddr* to, int addrlen)

    // Recieve data
    // int recvFrom(int sockfd, char *buf, size_t nbytes, int flags, struct sockaddr* to, int addrlen)

    // Close socket
    // int close(int sockfd)
}
