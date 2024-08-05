// server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  // Required for read and write functions

#define BUFFER_SIZE 256
#define FILE_BUFFER_SIZE 2000

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[BUFFER_SIZE], fileContent[FILE_BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    FILE *fd;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("SERVER: Waiting for client...\n");
    
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    bzero(buffer, BUFFER_SIZE);
    n = read(newsockfd, buffer, BUFFER_SIZE - 1);
    if (n < 0)
        error("ERROR reading from socket");

    printf("SERVER: Requested file path: %s\n", buffer);

    // Open the file
    fd = fopen(buffer, "r");
    if (fd != NULL) {
        printf("SERVER: File %s found! Transferring contents...\n", buffer);

        bzero(fileContent, FILE_BUFFER_SIZE);
        size_t bytesRead = fread(fileContent, 1, FILE_BUFFER_SIZE - 1, fd);
        if (bytesRead < 0)
            error("ERROR reading file");

        fileContent[bytesRead] = '\0'; // Null-terminate the string
        fclose(fd);

        n = write(newsockfd, fileContent, bytesRead);
        if (n < 0)
            error("ERROR writing to socket");
    } else {
        printf("SERVER: File not found!\n");
        const char *errorMsg = "File not found!";
        n = write(newsockfd, errorMsg, strlen(errorMsg));
        if (n < 0)
            error("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
    
    return 0;
}


// ....client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>  // Include this for read and write functions

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char filepath[256], buf[3000];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    printf("Client online\n");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    printf("Server online\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Correct cast to sockaddr*
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error connecting to socket");

    printf("Client: Enter path with filename:\n");
    scanf("%s", filepath);

    n = write(sockfd, filepath, strlen(filepath));
    if (n < 0)
        error("Error writing to socket");

    bzero(buf, sizeof(buf));
    n = read(sockfd, buf, sizeof(buf) - 1);
    if (n < 0)
        error("Error reading from socket");

    printf("Client: Displaying from socket\n");
    printf("%s\n", buf);

    close(sockfd);
    return 0;
}
// OUTPUT : AT TERMINAL 1 : (SERVER)
// $ gcc 5server.c
// ./a.out 8000
// SERVER:Waiting for client....
// SERVER:hello.c 
// SERVER:hello.c found! 
// Transfering the contents ...
// File content hi
// hello
// uvce
// AT TERMINAL 2 : (CLIENT)
// $ gcc 5client.c
// $ ./a.out 127.0.0.1 8000
// client online
// server online
// client:enter path with filename:
// hello.c
// client:displaying from sockethi
// hello
// uvce

