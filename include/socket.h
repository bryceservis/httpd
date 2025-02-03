//
// Created by saethroua on 2/2/25.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>

void create_ssl_context(char certificate_path[], char private_key_path[], SSL_CTX **context);
void create_sockaddr_in(char *address, uint16_t port, struct sockaddr_in *servaddr);
void create_sockaddr_in6(char *address, uint16_t port, struct sockaddr_in6 *servaddr);
int create_socket_ipv4(struct sockaddr_in *servaddr);
int create_socket_ipv6(struct sockaddr_in6 *servaddr);
void ssl_socket_listen(int sockfd, SSL_CTX *context);

#endif