#ifndef SERVER_H
#define SERVER_H

#include <signal.h>

#include "../threadpool/threadpool.h"
#include "../socket/socket.h"
#include "../utils/utils.h"
#include "../http/http.h"

extern volatile sig_atomic_t running;

void handle_sigint(int sig);

void* handle_connection(void*  connection_fd);

#endif