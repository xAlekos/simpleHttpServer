#ifndef URI_H
#define URI_H

#include "../utils/utils.h"
#define ROOT_DIR "/home/bantino/Progetti/simpleHttpServer/root"

typedef struct uri{

    char* path;
    char* extension;

}uri_t;

uri_t* uri_alloc();

void uri_free(uri_t* uri);

int uri_parse(char* uri_str, uri_t* uri);

#endif