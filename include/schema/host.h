/*

    Host Schema

*/

#ifndef HOST_H
#define HOST_H

#include <stdint.h>
#include "socket.h"

typedef struct {

    socket_list_t *sockets;
    size_t buffer_size;
    char *index;
    char *name;
    char *root;

} host_t;

#endif