#include "server.h"


void handle_sigint(int sig) {
    
    write(pipe_fd[1],"q",1);
    running = 0; 

}


void* handle_connection(void* connection_fd){

    char request_buff[REQUEST_BUFFER_SIZE] = {0};

    size_t bytes_read = read(*((int*)connection_fd), request_buff, sizeof(request_buff));
    
    if (bytes_read <= 0) {
        perror("[ERROR] Read failed");
        goto cleanup;
    } 

    printf("Request:\n %s", request_buff);
    http_request_status_line_t* sl = request_sl_alloc();
    if(sl == NULL){
        goto cleanup;
    }

    http_response_t* response = response_alloc();
    if(response == NULL)
        goto cleanup;

    if(request_status_line_parse(request_buff,sl) == 1){ //TODO Questa soluzione è temporanea, sostituire.

        response->status_line = response_status_line_create(BAD_REQUEST);
        
         http_response_send(response,*((int*)connection_fd));

        goto cleanup;

    }
    
    request_handle(response,sl);
    http_response_send(response,*((int*)connection_fd));

cleanup:
    request_sl_free(sl);
    response_free(response);
    close(*((int*)connection_fd));
    free(((int*)connection_fd));
    return NULL;
}

void server_loop(int socket_fd, thread_pool* pool){

    struct pollfd fds[2];

    fds[0].events = POLLIN;
    fds[0].fd = socket_fd;
    fds[1].events = POLLIN;
    fds[1].fd = pipe_fd[0];

    int poll_ret;
    

    while(running){


        poll_ret = poll(fds,2,-1);

        if(poll_ret == -1){
            if(errno == EINTR) continue;
            perror("[ERROR] Polling failed");
            running = 0;

        }
        
        else{

            if(fds[0].revents & POLLIN){

                int *connection_fd = malloc(sizeof(int));

                if (connection_fd == NULL) {
                    perror("[ERROR] connection file descriptor allocation failed: ");
                    continue;
                }

                *connection_fd = socket_accept(socket_fd);

                if (*connection_fd == -1) {
                    perror("[ERROR] Socket accept failed");
                    free(connection_fd);
                    continue;
                }
                
                add_work(handle_connection,(void*)connection_fd,pool);
                }

            if(fds[1].revents & POLLIN){
                running = 0;
            }

        }
        
    }


}


volatile sig_atomic_t running = 1;

int pipe_fd[2];


int main(){

    if(pipe(pipe_fd) == -1){
        perror("[ERROR] pipe creation failed");
    }
    

    signal(SIGINT, handle_sigint);
    int socket_fd = socket_init();
    thread_pool* pool = thread_pool_init();
    
    server_loop(socket_fd,pool);
    
    //loop finished

    printf("[INFO] Shutting down server!\n");

    thread_pool_shutdown(pool);
    close(socket_fd);
    return 0;
}
