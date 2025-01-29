//
// Created by saethroua on 1/29/25.
//
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "networking/socket.h"

int main(int argc, char *argv[]) {

    char address[] = "0.0.0.0";
    uint16_t port = 8080;
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-a") == 0) {

            if (argc <= i + 1) {

                printf("There must be an argument for the address parameter.\n");
                exit(EXIT_FAILURE);

            }

            strcpy(address, argv[i + 1]);

        } else if (strcmp(argv[i], "-p") == 0) {

            if (argc <= i + 1) {

                printf("There must be an argument for the port parameter.\n");
                exit(EXIT_FAILURE);

            }

            port = (uint16_t) strtol(argv[i + 1], NULL, 10);

            if (port <= 0) {

                printf("The port you have provided cannot be equal to or below zero.\n");
                exit(EXIT_FAILURE);

            }

        } else if (strcmp(argv[i], "-r") == 0) {

            if (argc <= i + 1) {

                printf("There must be an argument for the webroot parameter.\n");
                exit(EXIT_FAILURE);

            }

        } else if (strcmp(argv[i], "-h") == 0) {

            printf("-a  Sets the address to listen on\n");
            printf("-p  Sets the port to listen on\n");
            printf("-r  Sets the receive webroot\n");
            exit(EXIT_SUCCESS);

        }

    }

    struct sockaddr_in servaddr;
    sockaddr_in_helper(address, port, &servaddr);
    socket_helper(&servaddr);
    return 0;

}