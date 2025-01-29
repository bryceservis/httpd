#include "socket.h"

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
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

void *socket_worker(void *vargs) {

    int sockfd = (int) vargs;
    char buffer[MAX_BUFFER];
    recv(sockfd, buffer, MAX_BUFFER, 0);
    printf("%s\n", buffer);

    close(sockfd);
    return NULL;

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

    while (1) {

        int streamfd = accept(sockfd, (struct sockaddr *) servaddr, (socklen_t *) &length);
        if (streamfd == -1) {

            perror("Error accepting connection");
            exit(EXIT_FAILURE);

        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, socket_worker, (void *) streamfd);
        pthread_detach(thread_id);

    }

}