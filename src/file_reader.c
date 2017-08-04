#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>

char *readFileToCharStar(char *filename){
    //More or less copied and pasted from the cJSON examples
    
    // cJSON *result;
    FILE *f;
    long len;
    char *data;

    // open in read binary mode
    f = fopen(filename,"rb");
    if (f == NULL){
        return NULL;
    }
    
    // get the length
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    //allocate and read the data
    data = (char*)malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    
    fclose(f);
    
    return data;
}