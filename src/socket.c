/*

    Hypertext Transfer Protocol Daemon

*/

#include "../include/socket.h"
#include "../include/error.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <threads.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void create_ssl_context(char certificate_path[], char private_key_path[], SSL_CTX **context) {

    const SSL_METHOD *method = TLS_server_method();
    *context = SSL_CTX_new(method);
    if (!*context) {

        serror("SSL_CTX_new");
        exit(EXIT_FAILURE);

    }

    if (SSL_CTX_use_certificate_file(*context, certificate_path, SSL_FILETYPE_PEM) <= 0) {

        error("SSL_CTX_use_certificate_file", certificate_path);
        exit(EXIT_FAILURE);

    }

    if (SSL_CTX_use_PrivateKey_file(*context, private_key_path, SSL_FILETYPE_PEM) <= 0) {

        error("SSL_CTX_use_PrivateKey_file", private_key_path);
        exit(EXIT_FAILURE);

    }

}

void create_sockaddr_in(char *address, uint16_t port, struct sockaddr_in *servaddr) {

    if (inet_pton(AF_INET, address, &servaddr->sin_addr) == 0) {

        error("inet_pton", address);
        exit(EXIT_FAILURE);

    }

    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(port);

}

void create_sockaddr_in6(char *address, uint16_t port, struct sockaddr_in6 *servaddr) {

    if (inet_pton(AF_INET, address, &servaddr->sin6_addr) == 0) {

        error("inet_pton", address);
        exit(EXIT_FAILURE);

    }

    servaddr->sin6_family = AF_INET6;
    servaddr->sin6_port = htons(port);

}

int create_socket_ipv4(struct sockaddr_in *servaddr) {

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {

        serror("socket");
        exit(EXIT_FAILURE);

    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {

        serror("setsockopt");
        exit(EXIT_FAILURE);

    }

    if (bind(sockfd, (struct sockaddr *)servaddr, sizeof(struct sockaddr)) == -1) {

        serror("bind");
        exit(EXIT_FAILURE);

    }

    if (listen(sockfd, 1) == -1) {

        serror("listen");
        exit(EXIT_FAILURE);

    }

    return sockfd;

}

int create_socket_ipv6(struct sockaddr_in6 *servaddr) {

    int sockfd;
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0) == -1)) {

        serror("socket");
        exit(EXIT_FAILURE);

    }

    if (bind(sockfd, (struct sockaddr *)servaddr, sizeof(struct sockaddr)) == -1) {

        serror("bind");
        exit(EXIT_FAILURE);

    }

    if (listen(sockfd, 1) == -1) {

        serror("listen");
        exit(EXIT_FAILURE);

    }

    return sockfd;

}

void *handle_ssl_client(void *vargs) {

    SSL *ssl = (SSL *) vargs;
    printf("SSL_accept\n");
    char *response = "HTTP/1.1 200 OK\r\n";
    SSL_write(ssl, response, strlen(response));
    SSL_shutdown(ssl);
    SSL_free(ssl);

}

void ssl_socket_listen(int sockfd, SSL_CTX *context) {

    while (1) {

        struct sockaddr_in clientaddr;
        unsigned int length = sizeof(clientaddr);
        SSL *ssl = SSL_new(context);
        int clientfd;
        if ((clientfd = accept(sockfd, (struct sockaddr *) &clientaddr, &length)) == -1) {

            serror("accept");
            exit(EXIT_FAILURE);

        }

        SSL_set_fd(ssl, clientfd);
        if (SSL_accept(ssl) <= 0) {

            serror("SSL_accept");

        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_ssl_client, (void *) ssl);
        pthread_detach(thread_id);

    }

}