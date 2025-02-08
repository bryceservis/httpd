#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include "../include/error.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <stdbool.h>
#include <bits/getopt_core.h>
#include "../include/sockets.h"
#include "../include/config.h"
#include <arpa/inet.h>

typedef struct {

    host_t *host;
    socket_t *socket;

} master_thread_args;

void *master_thread(void *args) {

    if (!args) return NULL;
    master_thread_args *thread_args = (master_thread_args *) args;
    if (!thread_args->socket) return NULL;
    char buffer[INET6_ADDRSTRLEN];
    bool isIPv4 = true;
    if (inet_pton(AF_INET, thread_args->socket->address, buffer)) {

        isIPv4 = true;

    }

    struct sockaddr_in *servaddr_ipv4 = NULL;
    struct sockaddr_in6 *servaddr_ipv6 = NULL;
    int sockfd = -1;
    if (isIPv4) {

        struct sockaddr_in *servaddr = create_sockaddr_in(thread_args->socket->address, thread_args->socket->port);
        sockfd = create_socket_ipv4(servaddr);

    } else {

        struct sockaddr_in6 *servaddr = create_sockaddr_in6(thread_args->socket->address, thread_args->socket->port);
        sockfd = create_socket_ipv6(servaddr);

    }

    SSL_CTX *context = NULL;
    if (thread_args->socket->tls_conf) {

        context = create_tls_context(thread_args->socket->tls_conf);
        tls_socket_listen(context, sockfd, thread_args->host->buffer_size);
        return NULL;

    }

    socket_listen(sockfd, thread_args->host->buffer_size);
    return NULL;

}

int main(int argc, char *argv[]) {

    int option;
    char *config_file_path = NULL;
    while ((option = getopt(argc, argv, "c:h")) != -1) {

        switch (option) {

            case 'c':
                config_file_path = optarg;
                break;

            case 'h':
                printf("Usage: %s [-c config_file_path]\n", argv[0]);
                break;

            case '?':
                printf("Unrecognized option: %c\n", optopt);
                break;

        }

    }

    /*if (!config_file_path) {

        printf("Configuration file not specified\n");
        return 0;

    }*/

    host_t *config_file = create_host("../conf/example.conf");
    master_thread_args *thread_args = (master_thread_args *) malloc(sizeof(master_thread_args));
    thread_args->host = config_file;
    thread_args->socket = thread_args->host->sockets->socket;
    pthread_t thread;
    pthread_create(&thread, NULL, master_thread, (void *) thread_args);
    pthread_join(thread, NULL);
    free(thread_args);
    return 0;

}