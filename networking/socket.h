#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef SOCKET_H
#define SOCKET_H

void sockaddr_in_helper(char ip[], uint16_t port, struct sockaddr_in *servaddr);
void socket_helper(struct sockaddr_in *servaddr);

#endif
