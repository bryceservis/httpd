/*

    Config Header

*/

#ifndef CONFIG_C_H
#define CONFIG_C_H

#include <stdio.h>

typedef enum {

    SOCKET,
    SOCKET_KEEP_ALIVE_DURATION,
    SOCKET_ADDRESS,
    SOCKET_PORT,
    SOCKET_TLS,
    SOCKET_TLS_MINIMUM_VERSION,
    SOCKET_TLS_MAXIMUM_VERSION,
    SOCKET_TLS_CACHE,
    SOCKET_TLS_CACHE_SIZE,
    SOCKET_TLS_CACHE_DURATION,
    INDEX,
    ROOT,
    HOST,
    UNKNOWN

} config_types_t;

void create_host(FILE *config_file);

#endif
