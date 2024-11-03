#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include "../socket/socket.h"

void* handle_connection(void*  connection_fd);

#endif