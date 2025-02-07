#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <bits/getopt_core.h>
#include "../include/sockets.h"
#include "../include/config.h"

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

    if (!config_file_path) {

        printf("Configuration file not specified\n");
        return 0;

    }

    host_t *host = create_host(config_file_path);
    printf(host->sockets->socket->address);
    printf(host->sockets->next->socket->address);
    printf(host->sockets->next->next->socket->address);
    printf(host->sockets->socket->tls_conf->tls_cache->cache_id);
    //TODO: MEMORY CLEANUP FOR HOST STRUCTURE.
    return 0;

}