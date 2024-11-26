#include "http.h"


http_request_method_t parse_http_method(const char* method_str){
    
    if (strcmp(method_str, "GET") == 0) return GET;
    if (strcmp(method_str, "POST") == 0) return POST;
    if (strcmp(method_str, "CONNECT") == 0) return CONNECT;
    if (strcmp(method_str, "DELETE") == 0) return DELETE;
    if (strcmp(method_str, "HEAD") == 0) return HEAD;
    if (strcmp(method_str, "OPTIONS") == 0) return OPTIONS;
    if (strcmp(method_str, "PATCH") == 0) return PATCH;
    if (strcmp(method_str, "PUT") == 0) return PUT;
    if (strcmp(method_str, "TRACE") == 0) return TRACE;
    
    return METHOD_ERROR;  
}


http_request_version_t parse_http_version(const char* version_str){

    if (strcmp(version_str, "HTTP/1.0") == 0) return HTTP10;
    if (strcmp(version_str, "HTTP/1.1") == 0) return HTTP11;
    if (strcmp(version_str, "HTTP/2") == 0) return HTTP2;

    return VERSION_ERROR; 
}



http_request_status_line_t* request_sl_alloc(){

    struct http_request_status_line* sl = malloc(sizeof(http_request_status_line_t));

    if(sl == NULL){
        perror("[ERROR] http_request_status_line memory allocation failed");
        return NULL;
    }

    sl->uri = NULL; 

    return sl;

}

void request_sl_free(http_request_status_line_t* sl){
    
    if(sl->uri != NULL)
        uri_free(sl->uri);
    
    free(sl);
}


int request_status_line_parse(char* request_buffer, http_request_status_line_t* sl){

    char buffer_cpy[REQUEST_BUFFER_SIZE]; //TODO Probabilmente non serve 
    memcpy(buffer_cpy,request_buffer,REQUEST_BUFFER_SIZE);  

    uri_t* uri = uri_alloc();
    char* saveptr;
    char* request_method;
    char* uri_string;
    char* http_version;

    request_method = strtok_r(buffer_cpy," ",&saveptr);
    
    if(request_method == NULL){
        goto errorcleanup;
    }

    uri_string = strtok_r(NULL," ",&saveptr);
    
    if(uri_string == NULL){
        goto errorcleanup;
    }
    
    if(uri_parse(uri_string,uri) == 1){
        goto errorcleanup;
    }
    
    http_version = strtok_r(NULL,"\r",&saveptr);
    if(http_version == NULL){
        goto errorcleanup;
    }
    
    sl->request_method = parse_http_method(request_method);
    sl->version = parse_http_version(http_version);
    sl->uri = uri;
    
    return 0;

errorcleanup:
    uri_free(uri);
    return 1;

}


http_response_t* response_alloc(){

    http_response_t* response = malloc(sizeof(http_response_t));
    
    if(response == NULL){
        perror("[ERROR] response allocation failed");
        return NULL;
    }
    
    response->body = NULL;
    response->headers = NULL;
    response->status_line = NULL;
    response->body_size = 0;

    return response;
};

void response_free(http_response_t* response){

    if(response->body != NULL)
        free(response->body);
    
    if(response->headers != NULL)
        free(response->headers);

    if(response->status_line != NULL)
        free(response->status_line);    

    free(response);


}

char* response_status_line_create(http_status_code_t code){
    
    char* response_status_line;
    char* response_status;   
    
    switch(code){

        case UNIMPLEMENTED:
            response_status = HTTP_RESPONSE_NOT_IMPLEMENTED;   
            break;
        case BAD_REQUEST:
            response_status = HTTP_RESPONSE_BAD_REQUEST; 
            break;
        case OK:
            response_status = HTTP_RESPONSE_OK; 
            break;
        case NOT_FOUND:
            response_status = HTTP_RESPONSE_NOT_FOUND; 
            break;
        case INTERNAL_SERVER_ERROR:
            response_status = HTTP_RESPONSE_INTERNAL_SERVER_ERROR;
        default:
            response_status = HTTP_RESPONSE_INTERNAL_SERVER_ERROR; //Eventualmente da cambiare
    }

    response_status_line = malloc(strlen(HTTP_VERSION_11) + strlen(response_status) + 1);
    
    if(response_status_line == NULL){
        perror("[ERROR] Status line allocation failed");
        return NULL;
    }

    strcpy(response_status_line,HTTP_VERSION_11);
    strcat(response_status_line,response_status);

    return response_status_line;

}


int request_handle(http_response_t* response, http_request_status_line_t* request){
    
    bool status_code_set = 0;
    http_status_code_t status_code;

    switch (request->version){
        case HTTP2:
            status_code = UNIMPLEMENTED;
            status_code_set = 1;
            break;
        case VERSION_ERROR:
            status_code = BAD_REQUEST;
            status_code_set = 1;
        default:
            break;
    }

    if(status_code_set == 0){

        switch(request->request_method){

            case GET: 
                status_code = http_get(response,request->uri);
                break;
            default: 
                status_code = UNIMPLEMENTED;
                break;
        }

    }

    response->status_line = response_status_line_create(status_code);
        
    if(response->status_line == NULL){
        perror("[ERROR] Errore allocazione status line risposta: ");
        return 1;
    }

    return 0;
    
}

char* headers_string_generate(http_mime_type_t content_type, size_t content_size){

    char* headers_string =  malloc(1024);
    
    if(headers_string == NULL){
        perror("[ERROR] Headers string allocation failed");
        return NULL;
    }

    headers_string[0] = '\0';

    if(content_type != MISSING){
        strcat(headers_string,HTTP_HEADER_CONTENT_TYPE);
        
        switch(content_type){
            case PLAIN:
                strcat(headers_string,HTTP_CONTENT_PLAIN_TEXT);
                break;
            case JSON:
                strcat(headers_string,HTTP_CONTENT_APPLICATION_JSON);
                break;
            case HTML:
                strcat(headers_string,HTTP_CONTENT_TEXT_HTML);
                break;
            case PNG:
                strcat(headers_string,HTTP_CONTENT_IMG_PNG);
                break;
            case JPEG:
                strcat(headers_string,HTTP_CONTENT_IMG_JPEG);
                break;
            case SVG:
                strcat(headers_string,HTTP_CONTENT_IMG_SVG);
                break;
            case GIF:
                strcat(headers_string,HTTP_CONTENT_IMG_GIF);
                break;    
        }
        strcat(headers_string,"\r\n");
    }

    char size_string[32];

    snprintf(size_string,2,"%zu",content_size);

    strcat(headers_string, HTTP_HEADER_CONTENT_SIZE);
    strcat(headers_string, size_string);
    strcat(headers_string,"\r\n");
    

    return headers_string;
}


http_status_code_t http_get(http_response_t* response, uri_t* uri){
    
    printf("HTTP_GET: CERCO IL FILE %s\n", uri->path);
    FILE* requested_file = fopen(uri->path,"rb");
    int ret;

    if(requested_file == NULL){
        
        if(errno == ENOENT) return NOT_FOUND;
        else return INTERNAL_SERVER_ERROR;
        
    }

    size_t file_size = open_file_size_get(requested_file);
    response->body_size = file_size;

    response->body = malloc(file_size);
    
    if(response->body == NULL){
        perror("[ERROR] Response body allocation failed");
        fclose(requested_file);
        return INTERNAL_SERVER_ERROR;
    }

    ret = file_content_copy(requested_file,file_size,response->body);
    
    if(ret == 1){
        perror("[ERROR] File content copy to response body failed");
        free(response->body);
        response->body = NULL;
        fclose(requested_file);
        return INTERNAL_SERVER_ERROR;
    }

    char* headers_string = "Content Type: text/plain\r\n"; //TODO questa roba è temporanea.

    response->headers = malloc(strlen(headers_string) + 1);

    if(response->headers == NULL){
        perror("[ERROR] Response headers allocation failed");
        return INTERNAL_SERVER_ERROR;
    }

    strcpy(response->headers, headers_string);
    fclose(requested_file);
    
    return OK;


}
