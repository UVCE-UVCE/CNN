SERVER@!!!!!!!
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // Required for write function

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock, length, fromlen, n;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("Opening socket");
    }

    length = sizeof(server);
    memset(&server, 0, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        error("Binding");
    }

    fromlen = sizeof(from);
    while (1) {
        n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) {
            error("recvfrom");
        }

        write(1, "Received a datagram: ", 21);
        write(1, buf, n);

        n = sendto(sock, "Got your message\n", 17, 0, (struct sockaddr *)&from, fromlen);
        if (n < 0) {
            error("sendto");
        }
    }

    close(sock); // Added to close the socket before exiting
    return 0;
}
CLIENT!!!!!
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // Required for write function

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock, length, n;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == NULL) {
        error("Unknown host");
    }
    
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(server);

    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    
    // Use struct sockaddr for sendto and recvfrom
    n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length);
    if (n < 0) {
        error("sendto");
    }

    n = recvfrom(sock, buffer, 255, 0, (struct sockaddr *)&from, &length);
    if (n < 0) {
        error("recvfrom");
    }

    write(1, "Got an ack: ", 12);
    write(1, buffer, n);

    close(sock); // Close the socket
    return 0;
}
OUTPUT : AT TERMINAL 1 : (SERVER)
$ gcc 9server.c
~$ ./a.out 8080
Received a datagram: network
Received a datagram: lab
AT TERMINAL 2 : (CLIENT)
$ gcc 9client.c
$ ./a.out localhost 8080
Please enter the message: network
Got an ack: Got your message
$ ./a.out localhost 8080
Please enter the message: lab
Got an ack: Got your message
