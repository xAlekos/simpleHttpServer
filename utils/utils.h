#ifndef UTILS_H
#define UTILS_H

#define STRINGIFY(x) #x




#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h>
#include <stdbool.h>

#define REQUEST_BUFFER_SIZE 10000

size_t open_file_size_get(FILE* file);

int file_content_copy(FILE* file, size_t size ,char* buffer);


#endif