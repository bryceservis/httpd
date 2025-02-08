//
// Created by saethroua on 2/3/25.
//

#ifndef SOCKETS_H
#define SOCKETS_H

#include <netinet/in.h>
#include <stdint.h>
#include <openssl/ssl.h>

typedef struct {

    unsigned int cache_duration;
    size_t cache_size;
    char *cache_id;

} tls_cache_t;

typedef struct {

    tls_cache_t *tls_cache;
    char *certificate;
    char *private_key;
    char* minimum_version;
    char* maximum_version;

} tls_conf_t;

typedef struct {

    struct sockaddr_in *clientaddr;
    size_t buffer_size;
    SSL *tls_socket;

} tls_worker_thread_args_t;

typedef struct {

    struct sockaddr_in *clientaddr;
    size_t buffer_size;
    int clientfd;

} worker_thread_args_t;

struct sockaddr_in *create_sockaddr_in(char *ipv4, uint16_t port);
struct sockaddr_in6 *create_sockaddr_in6(char *ipv6, uint16_t port);
int create_socket_ipv4(struct sockaddr_in *servaddr);
int create_socket_ipv6(struct sockaddr_in6 *servaddr);
void socket_listen(int sockfd, size_t buffer_size);
void tls_socket_listen(SSL_CTX *tls_context, int sockfd, size_t buffer_size);
SSL_CTX *create_tls_context(tls_conf_t *tls_conf);

#endif //SOCKETS_H
