
#include "socket.h"


int socket_init(){

    int socket_fd;
    struct in_addr ipv4address = { .s_addr = htonl(INADDR_ANY) };
    struct sockaddr_in socket_address = { .sin_addr = ipv4address, .sin_port = htons(PORT), .sin_family = AF_INET };

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Errore apertura porta: ");
        exit(EXIT_FAILURE);
    }

    if (bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) != 0) {
        perror("Errore binding socket: ");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, BACKLOG) != 0) {
        perror("Errore messa in ascolto sulla socket: ");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Socket in ascolto su %s:%d\n", inet_ntoa(ipv4address), PORT);
    return socket_fd;

}

int socket_accept(int socket_fd){

    int connection_fd;
    struct sockaddr_in client_address;
    socklen_t connection_address_length;

    connection_address_length = sizeof(client_address);
    
    connection_fd = accept(socket_fd, (struct sockaddr*)&client_address, &connection_address_length);
    if (connection_fd < 0) {
        perror("Connessione fallita: ");
        close(socket_fd);
        return 1;
    }

    printf("Connessione stabilita con il client su %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

   return connection_fd;
   

}

