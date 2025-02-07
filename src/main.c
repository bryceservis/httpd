#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include "../include/sockets.h"
#include "../include/config.h"

int main(void) {
    FILE *file = fopen("../conf/example.conf", "r");
    create_host(file);
    return 0;
}