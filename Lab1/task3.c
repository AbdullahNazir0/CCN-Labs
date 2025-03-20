// Task:
// Write a C program to find the host byte order (little endian or big endian) of your
// machine. Your program must output the byte order of your machine along with the data
// stored in the individual bytes; e.g, you can store a number in hex, as 0x3412, and visualize
// how data is stored in the memory (either in little or big endian format).

// Functions to convert Endianness
// 1. htons() -> Convert short int from host to network byte order
// 2. htonl() -> Convert long int from host to network byte order
// 3. ntohs() -> Convert short int from network to host byte order
// 4. ntohl() -> Convert long int from network to host byte order

#include <stdio.h>
#include <arpa/inet.h> // header file for above conversion functions.

int main() {

    int hex = 0x3412;
    char *casted = (char *)&hex;

    printf("Memory Representation: ");
    for (int i = 0; i < sizeof(hex); i++) {
        printf("%02X ", casted[i]);
    }
    printf("\n");

    if (casted[0] == 0x12) {
        printf("System is Little Endian\n");
    } else if (casted[0] == 0x34) {
        printf("System is Big Endian\n");
    } else {
        printf("Unknown Endian Format\n");
    }

    return 0;
}
