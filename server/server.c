#include "server.h"


void* handle_connection(void* connection_fd){

    char request_buff[REQUEST_BUFFER_SIZE] = {0};

    read(*((int*)connection_fd), request_buff, sizeof(request_buff)); 

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

    if(response->status_line != NULL){
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

int main(){

    
    int socket_fd = socket_init();
    thread_pool* pool = thread_pool_init();
    bool running = true;

    while(running){

        int *connection_fd = malloc(sizeof(int));
        if (connection_fd == NULL) {
            perror("[ERROR] connection file descriptor allocation failed: ");
            close(socket_fd);
            running = false; //Qui si potrebbe pure non uccidere il server ma smettere di accettare connessioni ma per ora ci piace così 
        }
    
        *connection_fd = socket_accept(socket_fd);

        add_work(handle_connection,(void*)connection_fd,pool);

        
    }
    
    thread_pool_shutdown(pool);
    close(socket_fd);
    return 0;
}
