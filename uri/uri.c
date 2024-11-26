#include "uri.h"

uri_t* uri_alloc(){

    uri_t* uri = malloc(sizeof(uri_t));

    if(uri == NULL){
        perror("[ERROR] uri allocation failed");
        return NULL;
    }

    uri->extension = NULL;
    uri->path = NULL;

    return uri;

}

void uri_free(uri_t* uri){

    if(uri->extension != NULL)
        free(uri->extension);
    if(uri->path != NULL)
        free(uri->path);
    
    free(uri);
    
}

int uri_parse(char* uri_str,uri_t* uri){

    char* saveptr;
    char* str_segment = NULL;
    char* str_segment_prev = NULL;
    char* default_dir = "./root"; //TODO, renderla configurabile magari
    
    uri->path = malloc(strlen(default_dir) + strlen(uri_str) + 1);
        
    if(uri->path == NULL){
        perror("[ERROR] error allocating uri_path");
        return 1;
    }
    strcpy(uri->path, default_dir);    
    strcat(uri->path, uri_str);
    printf("uri_path parsed by URI_PARSE : %s\n",uri->path);
        
    str_segment = strtok_r(uri_str,".",&saveptr); 
    while((str_segment = strtok_r(NULL,".",&saveptr)) != NULL)
        str_segment_prev = str_segment;


    if(str_segment_prev != NULL){
        uri->extension = malloc(strlen(str_segment_prev) + 1);

        strcpy(uri->extension,str_segment_prev);
        printf("uri_extension parsed by URI_PARSE : %s\n",uri->extension);
        
        if(uri->extension == NULL){
            perror("[ERROR] error allocating uri_extension\n");
            return 1;
        }
    }

    else
        printf("Uri extension failed or missing extension\n");

    
    return 0;
    
}
