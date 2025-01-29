#include "socket.h"

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void sockaddr_in_helper(char ip[], uint16_t port, struct sockaddr_in *servaddr) {

    servaddr->sin_family = AF_INET;
    if (inet_aton(ip, &servaddr->sin_addr) == 0) {

        printf("The address you have provided is incorrect.\n");
        exit(EXIT_FAILURE);

    }

    servaddr->sin_port = htons(port);

}

void socket_worker(struct sockaddr_in *servaddr, int sockfd, int *length) {

    int netsockfd = accept(sockfd, (struct sockaddr *) servaddr, (socklen_t *) length);
    if (netsockfd == -1) {

        perror("Error accepting connection");
        exit(EXIT_FAILURE);

    }

    char buffer[MAX_TCP_BUFFER];
    read(netsockfd, buffer, MAX_TCP_BUFFER);
    send(netsockfd, "Test string", strlen("Test string"), 0);
    close(netsockfd);
    socket_worker(servaddr, sockfd, length);

}

void socket_helper(struct sockaddr_in *servaddr) {

    int option = 1;
    int length = sizeof(*servaddr);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {

        perror("Error bind");
        exit(EXIT_FAILURE);

    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1) {

        perror("Error setting socket options");
        exit(EXIT_FAILURE);

    }

    if (bind(sockfd, (struct sockaddr *) servaddr, length) == -1) {

        perror("Error binding");
        exit(EXIT_FAILURE);

    }

    if (listen(sockfd, 4096) == -1) {

        perror("Error listen");
        exit(EXIT_FAILURE);

    }

    socket_worker(servaddr, sockfd, &length);

}
