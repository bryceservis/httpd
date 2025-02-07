#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"
#include "../include/error.h"
#include "../include/schema/host.h"

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

void sanitize(char *string) {

    int j = 0;
    for (int i = 0; string[i] != '\0'; i++) {

        if (string[i] != ' ') {

            string[j] = string[i];
            j++;

        }

    }

    string[j] = '\0';

}

host_t *create_host(char *config_file_path) {

    FILE *config_file = fopen(config_file_path, "r");
    if (!config_file) {

        error("Error opening configuration file");

    }

    char *line = NULL;
    host_t *host = NULL;
    size_t line_length = 0;
    unsigned int line_number = 0;
    socket_list_t *newest_socket_list = NULL;
    while (getline(&line, &line_length, config_file) != -1) {

        line_number++;
        char *type_string = strtok(line, "=");
        char *value_string = strtok(NULL, "=");
        if (!type_string || !value_string) {

            continue;

        }

        sanitize(type_string);
        sanitize(value_string);
        config_types_t type = match_type(type_string);
        switch (type) {

            case HOST:
                host = (host_t *) malloc(sizeof(host_t));
                break;

            case ROOT:
                if (!host) break;
                host->root = strdup(value_string);
                break;

            case INDEX:
                if (!host) break;
                host->index = strdup(value_string);
                break;

            case SOCKET:
                if (!host) break;
                if (!host->sockets || !newest_socket_list) {

                    host->sockets = (struct socket_list *) malloc(sizeof(host_t));
                    host->sockets->socket = (socket_t *) malloc(sizeof(socket_t));
                    newest_socket_list = host->sockets;
                    break;

                }

                newest_socket_list->next = (socket_list_t *) malloc(sizeof(socket_list_t));
                newest_socket_list = newest_socket_list->next;
                newest_socket_list->socket = (socket_t *) malloc(sizeof(socket_t));
                break;

            case SOCKET_ADDRESS:
                if (!newest_socket_list || !newest_socket_list->socket) break;
                newest_socket_list->socket->address = strdup(value_string);
                break;

            case SOCKET_PORT:
                if (!newest_socket_list || !newest_socket_list->socket) break;
                newest_socket_list->socket->port = strtol(value_string, NULL, 10);
                break;

            case SOCKET_KEEP_ALIVE_DURATION:
                if (!newest_socket_list || !newest_socket_list->socket) break;
                newest_socket_list->socket->keep_alive_duration = strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS:
                if (!newest_socket_list || !newest_socket_list->socket) break;
                newest_socket_list->socket->tls_conf = (tls_conf_t *) malloc(sizeof(tls_conf_t));
                break;

            case SOCKET_TLS_MINIMUM_VERSION:
                if (!newest_socket_list || !newest_socket_list->socket || !newest_socket_list->socket->tls_conf) break;
                newest_socket_list->socket->tls_conf->minimum_version = (int) strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS_MAXIMUM_VERSION:
                if (!newest_socket_list || !newest_socket_list->socket || !newest_socket_list->socket->tls_conf) break;
                newest_socket_list->socket->tls_conf->maximum_version = (int) strtol(value_string, NULL, 10);
                break;

            case SOCKET_TLS_CACHE:
                if (!newest_socket_list || !newest_socket_list->socket || !newest_socket_list->socket->tls_conf) break;
                newest_socket_list->socket->tls_conf->tls_cache = (tls_cache_t *) malloc(sizeof(tls_conf_t));
                newest_socket_list->socket->tls_conf->tls_cache->cache_id = "microhttpd";
                break;

            case SOCKET_TLS_CACHE_DURATION:
                if (!newest_socket_list || !newest_socket_list->socket || !newest_socket_list->socket->tls_conf || !newest_socket_list->socket->tls_conf->tls_cache->cache_size) break;
                newest_socket_list->socket->tls_conf->tls_cache->cache_duration = strtol(value_string, NULL, 10);
                break;


            default:
                break;

        }

    }

    fclose(config_file);
    if (line) {

        free(line);

    }

    return host;

}