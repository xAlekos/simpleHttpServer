#include "utils.h"





size_t open_file_size_get(FILE* file){
    
    size_t file_size;
    fseek(file,0,SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return file_size;
}


int file_content_copy(FILE* file, size_t size ,char* buffer){

    int ret;    
    ret = fread(buffer,1,size,file);
    
    if(ret != size){
        printf("[ERROR] File content copy failed");
        return 1;
    }
    
    return 0;

}

