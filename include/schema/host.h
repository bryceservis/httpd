/*

    Host Schema

*/

#ifndef HOST_H
#define HOST_H

#include <stdint.h>
#include "socket.h"

typedef struct {

    socket_list_t *sockets;
    char *index;
    char *root;

} host_t;

#endif