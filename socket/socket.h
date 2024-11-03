#ifndef SOCKET_H
#define SOCKET_H
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include "../utils/utils.h"
#define BACKLOG 5
#define PORT 9090


int socket_init();

int socket_accept(int socket_fd);


#endif