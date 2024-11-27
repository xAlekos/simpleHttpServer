#include "mime_type.h"

http_mime_type_t mime_type_from_extension(char* extension){

    if(extension == NULL)
        printf("[INFO] mime_type_from_extension(mime_type.h) extension missing\n");

    else
        printf("[INFO] mime_type_from_extension(mime_type.h) %s\n",extension);

    if(extension == NULL || strcmp(extension,"txt") == 0)
        return PLAIN;

    if(strcmp(extension,"json") == 0)
        return JSON;
    
    if(strcmp(extension,"js") == 0)
        return JAVASCRIPT;

    if(strcmp(extension,"html")  == 0)
        return HTML;

    if(strcmp(extension,"jpeg")  == 0 || strcmp(extension,"jpg")  == 0)
        return JPEG;
    
    if(strcmp(extension,"png") == 0)
        return PNG;

    if(strcmp(extension,"svg") == 0)
        return SVG;

    if(strcmp(extension,"gif") == 0)
        return GIF;

    return MISSING;

}
