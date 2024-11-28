#ifndef SERVER_H
#define SERVER_H

#include "../threadpool/threadpool.h"
#include "../socket/socket.h"
#include "../utils/utils.h"
#include "../http/http.h"

void* handle_connection(void*  connection_fd);

#endif