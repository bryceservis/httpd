/*

    Sockets

*/

#include "../include/sockets.h"
#include "../include/error.h"
#include "../include/http.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct sockaddr_in *create_sockaddr_in(char *ipv4, uint16_t port) {

    struct sockaddr_in *servaddr = malloc(sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(port);
    if (inet_pton(AF_INET, ipv4, &servaddr->sin_addr) <= 0) {

        error("Malformed IPv4 address");

    }

    return servaddr;

}

struct sockaddr_in6 *create_sockaddr_in6(char *ipv6, uint16_t port) {

    struct sockaddr_in6 *servaddr = malloc(sizeof(struct sockaddr_in6));
    servaddr->sin6_scope_id = 0;
    servaddr->sin6_flowinfo = 0;
    servaddr->sin6_family = AF_INET6;
    servaddr->sin6_port = htons(port);
    if (inet_pton(AF_INET6, ipv6, &servaddr->sin6_addr) <= 0) {

        error("Malformed IPv6 address");

    }

    return servaddr;

}

int create_socket_ipv4(struct sockaddr_in *servaddr) {

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {

        error("Error creating socket");

    }

    int option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1) {

        error("Error setting socket options");

    }

    if (bind(sockfd, (struct sockaddr *) servaddr, sizeof(*servaddr)) == -1) {

        error("Error on bind");

    }

    if (listen(sockfd, 4096) == -1) {

        error("Error on listen");

    }

    return sockfd;

}

int create_socket_ipv6(struct sockaddr_in6 *servaddr) {

    int sockfd;
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1) {

        error("Error creating socket");

    }

    int option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) == -1) {

        error("Error setting socket options");

    }

    if (bind(sockfd, (struct sockaddr *) servaddr, sizeof(*servaddr)) == -1) {

        error("Error on bind");

    }

    if (listen(sockfd, 4096) == -1) {

        error("Error on listen");

    }

    return sockfd;

}

SSL_CTX *create_tls_context(char *certificate_path, char *private_key_path, tls_conf_t *tls_conf) {

    const SSL_METHOD *tls_method = TLS_server_method();
    SSL_CTX *tls_context = SSL_CTX_new(tls_method);
    if (!tls_context) {

        error("Error creating TLS context");

    }

    if (!SSL_CTX_set_min_proto_version(tls_context, tls_conf->minimum_version)) {

        error("Error setting TLS min version");

    }

    if (!SSL_CTX_set_max_proto_version(tls_context, tls_conf->maximum_version)) {

        error("Error setting TLS max version");

    }

    SSL_CTX_set_options(tls_context, SSL_OP_IGNORE_UNEXPECTED_EOF | SSL_OP_NO_RENEGOTIATION | SSL_OP_CIPHER_SERVER_PREFERENCE);
    if (SSL_CTX_use_certificate_chain_file(tls_context, certificate_path) == 0) {

        error("Error loading certificate file");

    }

    if (SSL_CTX_use_PrivateKey_file(tls_context, private_key_path, SSL_FILETYPE_PEM) == 0) {

        error("Error loading private key file");

    }

    if (tls_conf->tls_cache) {

        SSL_CTX_set_session_id_context(tls_context, (void *) tls_conf->tls_cache->cache_id, sizeof(tls_conf->tls_cache->cache_id));
        SSL_CTX_set_session_cache_mode(tls_context, SSL_SESS_CACHE_SERVER);
        SSL_CTX_sess_set_cache_size(tls_context, tls_conf->tls_cache->cache_size);
        SSL_CTX_set_timeout(tls_context, tls_conf->tls_cache->timeout_duration);
        SSL_CTX_set_verify(tls_context, SSL_VERIFY_NONE, NULL);

    }

    return tls_context;

}

void *worker_thread(void *args) {

    worker_thread_args_t *worker_thread_args = (worker_thread_args_t *) args;

    shutdown(worker_thread_args->clientfd, SHUT_RDWR);
    close(worker_thread_args->clientfd);
    free(worker_thread_args);
    return NULL;

}

void *tls_worker_thread(void *args) {

    tls_worker_thread_args_t *tls_worker_thread_args = (tls_worker_thread_args_t *) args;
    char *request = (char *) malloc(tls_worker_thread_args->buffer_size);
    SSL_read(tls_worker_thread_args->tls_socket, request, (int) tls_worker_thread_args->buffer_size);
    size_t length;
    SSL_write(tls_worker_thread_args->tls_socket, serve(request, tls_worker_thread_args->buffer_size), (int) tls_worker_thread_args->buffer_size);
    SSL_shutdown(tls_worker_thread_args->tls_socket);
    SSL_free(tls_worker_thread_args->tls_socket);
    free(tls_worker_thread_args);
    return NULL;

}

void socket_listen(int sockfd, size_t buffer_size) {

    while (1) {

        int clientfd;
        struct sockaddr_in clientaddr;
        unsigned int clientaddr_size = sizeof(clientaddr);
        if ((clientfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddr_size)) == -1) {

            error("Error on accept");

        }

        worker_thread_args_t *worker_thread_args = malloc(sizeof(worker_thread_args_t));
        worker_thread_args->buffer_size = buffer_size;
        worker_thread_args->clientaddr = &clientaddr;
        worker_thread_args->clientfd = clientfd;
        pthread_t worker_thread_id;
        pthread_create(&worker_thread_id, NULL, worker_thread, worker_thread_args);
        pthread_detach(worker_thread_id);

    }

}

void tls_socket_listen(SSL_CTX *tls_context, int sockfd, size_t buffer_size) {

    while (1) {

        int clientfd;
        struct sockaddr_in clientaddr;
        unsigned int clientaddr_size = sizeof(clientaddr);
        SSL *tls_socket = SSL_new(tls_context);
        if ((clientfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientaddr_size)) == -1 ) {

            error("Error on accept");

        }

        if (SSL_set_fd(tls_socket, clientfd) == 0) {

            error("Error setting TLS fd");

        }

        if (SSL_accept(tls_socket) == 0) {

            error("Error on TLS accept");

        }

        tls_worker_thread_args_t *tls_worker_thread_args = malloc(sizeof(tls_worker_thread_args_t));
        tls_worker_thread_args->buffer_size = buffer_size;
        tls_worker_thread_args->clientaddr = &clientaddr;
        tls_worker_thread_args->tls_socket = tls_socket;
        pthread_t tls_worker_thread_id;
        pthread_create(&tls_worker_thread_id, NULL, tls_worker_thread, (void *) tls_worker_thread_args);
        pthread_detach(tls_worker_thread_id);

    }

}