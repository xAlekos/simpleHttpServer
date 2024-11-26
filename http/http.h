#ifndef HTTP_H
#define HTTP_H
#include "../utils/utils.h"
#include "../uri/uri.h"


#define HTTP_VERSION_11 "HTTP/1.1 "

#define HTTP_RESPONSE_OK "200 OK\r\n"
#define HTTP_RESPONSE_NOT_FOUND "404 Not Found\r\n"
#define HTTP_RESPONSE_NOT_IMPLEMENTED "501 Not Implemented\r\n"
#define HTTP_RESPONSE_BAD_REQUEST "400 Bad Request\r\n"
#define HTTP_RESPONSE_INTERNAL_SERVER_ERROR "500 Internal Server Error\r\n"

#define HTTP_HEADER_CONTENT_TYPE "Content Type: "
#define HTTP_HEADER_CONTENT_SIZE "Content Size: "


#define HTTP_CONTENT_PLAIN_TEXT "text/plain"
#define HTTP_CONTENT_APPLICATION_JSON "application/json"
#define HTTP_CONTENT_TEXT_HTML "text/html"
#define HTTP_CONTENT_IMG_PNG "image/png"
#define HTTP_CONTENT_IMG_JPEG "image/jpeg"
#define HTTP_CONTENT_IMG_SVG "image/svg+xml"
#define HTTP_CONTENT_IMG_GIF "image/gif"


typedef enum http_mime_type{
    MISSING,
    PLAIN,
    JSON,
    HTML,
    PNG,
    JPEG,
    SVG,
    GIF

}http_mime_type_t;

typedef enum http_request_method{
    GET,
    POST,
    CONNECT,
    DELETE,
    HEAD,
    OPTIONS,
    PATCH,
    PUT,
    TRACE,
    METHOD_ERROR
    
}http_request_method_t; 


http_request_method_t parse_http_method(const char* method_str);

typedef enum http_request_version{
    
    HTTP10,
    HTTP11,
    HTTP2,
    VERSION_ERROR

}http_request_version_t; 

http_request_version_t parse_http_version(const char* version_str);

typedef enum http_status_code{
    
    OK,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR,
    BAD_REQUEST,
    UNIMPLEMENTED,
    CODE_ERROR

}http_status_code_t; 



typedef struct http_request_status_line{

    http_request_method_t request_method;
    uri_t* uri;
    http_request_version_t version;

}http_request_status_line_t;

struct http_request_status_line* request_sl_alloc();

void request_sl_free(http_request_status_line_t* sl);

int request_status_line_parse(char* request_buffer,http_request_status_line_t* sl);


typedef struct http_response{

    char* status_line;
    char* headers;
    char* body;
    size_t body_size;

}http_response_t;

http_response_t* response_alloc();

void response_free(http_response_t *response);

int request_handle(http_response_t* response, http_request_status_line_t* request);

char* response_status_line_create(http_status_code_t code);

http_status_code_t http_get(http_response_t* response, uri_t* uri);

#endif