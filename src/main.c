#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include "../include/sockets.h"
#include "../include/config.h"

int main(void) {

    host_t *t = create_host("../conf/example.conf");
    printf("%s", t->sockets->next->socket->address);
    return 0;
}