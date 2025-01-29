#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

void sockaddr_in_helper(char ip[], uint16_t port, struct sockaddr_in *servaddr) {

    servaddr->sin_family = AF_INET;
    if (inet_aton(ip, &servaddr->sin_addr) == 0) {

        printf("Malformed IP address\n");
        exit(EXIT_FAILURE);

    }

    servaddr->sin_port = htons(port);

}

void socket_worker(struct sockaddr_in *servaddr, int sockfd, int *length) {

    if (accept(sockfd, (struct sockaddr *) servaddr, (socklen_t *) length) == -1) {

        perror("Error accepting connection");
        exit(EXIT_FAILURE);

    }

}

void socket_helper(struct sockaddr_in *servaddr) {

    int length = sizeof(*servaddr);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {

        perror("Error bind");
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
