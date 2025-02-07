/*

    Socket Schema

*/

#ifndef SOCKET_H
#define SOCKET_H

#include "../sockets.h"

typedef struct {

    tls_conf_t *tls_conf;
    uint16_t keep_alive_duration;
    uint16_t port;
    char *address;

} socket_t;

typedef struct socket_list {

    struct socket_list *next;
    socket_t *socket;

} socket_list_t;

#endif