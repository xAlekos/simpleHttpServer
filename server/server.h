#ifndef SERVER_H
#define SERVER_H

#include <signal.h>
#include <poll.h>

#include "../threadpool/threadpool.h"
#include "../socket/socket.h"
#include "../utils/utils.h"
#include "../http/http.h"

extern volatile sig_atomic_t running;

extern int pipe_fd[2];

void handle_sigint(int sig);

void* handle_connection(void*  connection_fd);

void server_loop(int socket_fd, thread_pool* pool);

#endif