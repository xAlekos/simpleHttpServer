#ifndef MIME_TYPE_H
#define MIME_TYPE_H

#include "../utils/utils.h"

#define HTTP_CONTENT_PLAIN_TEXT "text/plain"
#define HTTP_CONTENT_APPLICATION_JSON "application/json"
#define HTTP_CONTENT_APPLICATION_JAVASCRIPT "application/javascript"
#define HTTP_CONTENT_TEXT_HTML "text/html"
#define HTTP_CONTENT_IMG_PNG "image/png"
#define HTTP_CONTENT_IMG_JPEG "image/jpeg"
#define HTTP_CONTENT_IMG_SVG "image/svg+xml"
#define HTTP_CONTENT_IMG_GIF "image/gif"


typedef enum http_mime_type{
    MISSING,
    PLAIN,
    JSON,
    JAVASCRIPT,
    HTML,
    PNG,
    JPEG,
    SVG,
    GIF

}http_mime_type_t;


http_mime_type_t mime_type_from_extension(char* extension);

#endif