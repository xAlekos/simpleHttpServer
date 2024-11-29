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
    http_response_t* response = response_alloc();

    if(request_status_line_parse(request_buff,sl) == 1){ //TODO Questa soluzione è temporanea, sostituire.

        response->status_line = response_status_line_create(BAD_REQUEST);
        
        if(response->status_line != NULL){ 
            write(*((int*)connection_fd),response->status_line,strlen(response->status_line));
            printf("%s",response->status_line);
        }

        goto cleanup;

    }
    
    request_handle(response,sl);
    
    printf("RISPOSTA:\n");

    if(response->status_line != NULL){  //TODO SOSTITUIRE CON FUNZIONE!
        write(*((int*)connection_fd),response->status_line,strlen(response->status_line));
        printf("%s",response->status_line);
    }
    if(response->headers != NULL){    
        write(*((int*)connection_fd),response->headers,strlen(response->headers));
        printf("%s",response->headers);
    }
    if(response->body != NULL){
        write(*((int*)connection_fd),"\n",1);
        printf("\n");
        write(*((int*)connection_fd),response->body,response->body_size);
    }

cleanup:
    request_sl_free(sl);
    response_free(response);
    close(*((int*)connection_fd));
    free(((int*)connection_fd));
    return NULL;
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
                    running = false;
                }

                *connection_fd = socket_accept(socket_fd);
                add_work(handle_connection,(void*)connection_fd,pool);
                }

            if(fds[1].revents & POLLIN){
                running = 0;
            }

        }
        
    }

    printf("[INFO] Shutting down server!\n");

    thread_pool_shutdown(pool);
    close(socket_fd);
    return 0;
}
