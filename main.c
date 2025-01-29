//
// Created by saethroua on 1/29/25.
//
#include "networking/socket.h"

void main() {

    struct sockaddr_in servaddr;
    sockaddr_in_helper("0.0.0.0", 8080, &servaddr);
    socket_helper(&servaddr);

}