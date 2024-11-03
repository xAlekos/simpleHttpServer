#include "server.h"
#include "../http/http.h"

void* handle_connection(void* connection_fd){

    char request_buff[10000] = {0};
    char response_buff[10000] = {0};

    read(*((int*)connection_fd), request_buff, sizeof(request_buff)); 

    printf("Messaggio dal client: %s", request_buff);
    memset(request_buff,0,sizeof(request_buff));

    valide_response_create(response_buff);
    write(*((int*)connection_fd),response_buff,strlen(response_buff));

    memset(response_buff,0,sizeof(response_buff));
    
    close(*((int*)connection_fd));
    free(((int*)connection_fd));
    return NULL;
}

int main(){

    
    int socket_fd = socket_init();

    while(1){

        int *connection_fd = malloc(sizeof(int));
        if (connection_fd == NULL) {
            perror("Errore allocazione memoria: ");
            close(socket_fd);
            return 0; //Qui si potrebbe pure non uccidere il server ma smettere di accettare connessioni ma per ora ci piace così
        }
    
        *connection_fd = socket_accept(socket_fd);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_connection, connection_fd) != 0) {
            perror("Errore creazione thread: ");
            close(socket_fd);
            return 0;
        }
    }
    
    close(socket_fd);
    return 0;
}
