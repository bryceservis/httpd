/*

    Config Header

*/

#ifndef CONFIG_C_H
#define CONFIG_C_H

#include <stdio.h>
#include "schema/host.h"

typedef enum {

    SOCKET,
    BUFFER_SIZE,
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
    NAME,
    SOCKET_TLS_CERTIFICATE,
    SOCKET_TLS_PRIVATE_KEY,
    UNKNOWN

} config_types_t;

host_t *create_host(char *config_file_path);

#endif
