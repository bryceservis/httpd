//
// Created by saethroua on 1/29/25.
//
#include "networking/socket.h"

void main() {

    struct sockaddr_in servaddr;
    sockaddr_in_helper("127.0.0.1", 8080, &servaddr);
    socket_helper(&servaddr);

}