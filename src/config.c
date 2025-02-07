#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"
#include "../include/schema/host.h"

char *remove_spaces(char *source) {

    int j = 0;
    for (int i = 0; source[i] != '\0'; i++) {

        if (source[i] != ' ') {

            source[j] = source[i];
            j++;

        }

    }

    source[j] = '\0';
    return source;

}

config_types_t match_type(char *type) {

    if (strcmp(type, "host") == 0) return HOST;
    else if (strcmp(type, "root") == 0) return ROOT;
    else if (strcmp(type, "index") == 0) return INDEX;
    else if (strcmp(type, "socket") == 0) return SOCKET;
    else if (strcmp(type, "address") == 0) return SOCKET_ADDRESS;
    else if (strcmp(type, "port") == 0) return SOCKET_PORT;
    else if (strcmp(type, "keep_alive_duration") == 0) return SOCKET_KEEP_ALIVE_DURATION;
    else if (strcmp(type, "tls") == 0) return SOCKET_TLS;
    else if (strcmp(type, "tls_cache") == 0) return SOCKET_TLS_CACHE;
    else if (strcmp(type, "tls_cache_size") == 0) return SOCKET_TLS_CACHE_SIZE;
    else if (strcmp(type, "tls_cache_duration") == 0) return SOCKET_TLS_CACHE_DURATION;
    else if (strcmp(type, "tls_minimum_version") == 0) return SOCKET_TLS_MINIMUM_VERSION;
    else if (strcmp(type, "tls_maximum_version") == 0) return SOCKET_TLS_MAXIMUM_VERSION;
    else return UNKNOWN;

}

void create_host(FILE *config_file) {

    size_t length = 0;
    int line_number = 0;
    char *config_line = NULL;
    host_t *host = NULL;
    int socket_counter = 0;
    while (getline(&config_line, &length, config_file) != -1) {

        line_number++;
        char *type_string = strtok(config_line, "=");
        char *value_string = strtok(NULL, "=");
        if (!type_string || !value_string) {

            continue;

        }

        if (type_string[0] == '\0' || value_string[0] == '\0') {

            continue;

        }

        socket_list_t *socket_list = NULL;
        config_types_t type = match_type(remove_spaces(type_string));
        switch (type) {

            case HOST:
                host = (host_t *) malloc(sizeof(host_t));
                break;

            case ROOT:
                host->root = strdup(value_string);
                break;

            case INDEX:
                host->index = strdup(value_string);
                break;

            case SOCKET:
                socket_list = host->sockets;
                if (!socket_list) {

                    host->sockets = (socket_list_t *) malloc(sizeof(socket_list_t));
                    host->sockets->socket = (socket_t *) malloc(sizeof(socket_t));
                    host->sockets->next = NULL;
                    break;

                }

                while (socket_list->next != NULL) {

                    socket_list = socket_list->next;

                }

                socket_list->next = (socket_list_t *) malloc(sizeof(socket_list_t));
                socket_list->next->socket = (socket_t *) malloc(sizeof(socket_t));
                socket_list->next->next = NULL;
                socket_counter++;
                break;

            case SOCKET_KEEP_ALIVE_DURATION:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->keep_alive_duration = strtol(value_string, NULL, 10);
                break;

            case SOCKET_ADDRESS:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->address = remove_spaces(value_string);
                break;

            case SOCKET_PORT:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->port = strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf = (tls_conf_t *) malloc(sizeof(tls_conf_t));
                break;

            case SOCKET_TLS_MINIMUM_VERSION:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf->minimum_version = (int) strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS_MAXIMUM_VERSION:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf->maximum_version = (int) strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS_CACHE:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf->tls_cache = (tls_cache_t *) malloc(sizeof(tls_cache_t));
                socket_list->socket->tls_conf->tls_cache->cache_id = "microhttpd";
                break;

            case SOCKET_TLS_CACHE_SIZE:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf->tls_cache->cache_size = (int) strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS_CACHE_DURATION:
                socket_list = host->sockets;
                for (int i = 0; i < socket_counter; i++) {

                    socket_list = socket_list->next;

                }

                socket_list->socket->tls_conf->tls_cache->cache_duration = (int) strtol(value_string, NULL, 10);
                break;

            default:
                break;

        }

    }

    fclose(config_file);
    if (config_line) {

        free(config_line);

    }

}