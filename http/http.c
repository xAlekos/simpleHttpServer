#include "http.h"

void valide_response_create(char* buffer){
    char* start_line = "HTTP/1.1 202 OK\n";
    char* headers = "Content-Length: 13\nContent-Type: text/plain\n\n";
    char* body = "Hello, world!";
    strcpy(buffer, start_line);
    strcat(buffer, headers);
    strcat(buffer, body);
}
