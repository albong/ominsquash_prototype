#include "text.h"
#include "logging.h"
#include "constants.h"
#include "file_reader.h"
#include "../lib/cJSON/cJSON.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static LanguageCode currentLanguageCode = EN;
static Text *textTable;
static size_t textTableLength;

static int readByteRangeToText(unsigned char *data, int startByte, int endByte, Text *result);

Text *init_Text(Text *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->length = 0;
    self->ids = NULL;
    
    LOG_INF("Text at %p initialized", self);
    return self;
}

void free_Text(Text *self){
    free(self->ids);
    free(self);
    LOG_INF("Text at %p freed", self);
}

void setCurrentLanguage(LanguageCode code){
    currentLanguageCode = code;
}

const char *getCurrentLanguageString(){
    char *result;
    
    switch (currentLanguageCode){
        case EN:
            return "en";
            break;
        
        case KO:
            return "ko";
            break;
        
        default:
            return "";
            break;
    }
}

void loadTextForCurrentLanguage(){
    char filename[FILENAME_BUFFER_SIZE];
    char manifestFilename[FILENAME_BUFFER_SIZE];
    
    //choose the filname based on the lanuage
    switch (currentLanguageCode){
        case EN:
            sprintf(filename, "data/text/text.en");
            sprintf(manifestFilename, "data/text/text.en.manifest");
            break;
        
        case KO:
            sprintf(filename, "data/text/text.ko");
            sprintf(manifestFilename, "data/text/text.ko.manifest");
            break;
        
        default:
            LOG_ERR("Language code not set!");
            sprintf(filename, "");
            sprintf(manifestFilename, "");
            break;
    }
    
    //read in the text file
    unsigned long fileLength;
    unsigned char *data = readBinaryFileToCharStar(filename, &fileLength);
    LOG_INF("Text file %s read", filename);
    
    //read in the manifest
    char *manifestContents = readFileToString(manifestFilename);
    cJSON *root = cJSON_Parse(manifestContents);
    if (!root){
        LOG_ERR("Failed to parse %s:\n%s", manifestFilename, cJSON_GetErrorPtr());
    } else {
        LOG_INF("Manifest %s read", manifestFilename);
    }
    
    //pull out relevant arrays
    cJSON *jsonIds = cJSON_GetObjectItem(root, "ids");
    cJSON *jsonStartByte = cJSON_GetObjectItem(root, "start byte");
    cJSON *jsonEndByte = cJSON_GetObjectItem(root, "end byte");
    
    //sanity check
    int textCount = cJSON_GetArraySize(jsonIds);
    if (cJSON_GetArraySize(jsonStartByte) != textCount || cJSON_GetArraySize(jsonEndByte) != textCount || cJSON_GetArraySize(jsonStartByte) != cJSON_GetArraySize(jsonEndByte)){
        LOG_WAR("Arrays in the text json file do not match!");
    }
    
    //determine how large to make the text array
    int largestId = -1;
    int i;
    for (i = 0; i < textCount; i++){
        if (cJSON_GetArrayItem(jsonIds, i)->valueint > largestId){
            largestId = cJSON_GetArrayItem(jsonIds, i)->valueint;
        }
    }
    Text *textById = calloc(largestId + 1, sizeof(Text)); //+1 since zero indexed
    
    //load text
    int id, startByte, endByte;
    for (i = 0; i < textCount; i++){
        id = cJSON_GetArrayItem(jsonIds, i)->valueint;
        startByte = cJSON_GetArrayItem(jsonStartByte, i)->valueint;
        endByte = cJSON_GetArrayItem(jsonEndByte, i)->valueint;

        if (!readByteRangeToText(data, startByte, endByte, textById + id)){
            LOG_ERR("Failed to decode UTF8 for id %d from byte %d to %d", id, startByte, endByte);
        }
    }
    LOG_INF("Text table created");
    
    //free everything
    free(data);
    cJSON_Delete(root);
    free(manifestContents);
    
    //set the variables
    textTable = textById;
    textTableLength = largestId + 1;
}

static int readByteRangeToText(unsigned char *data, int startByte, int endByte, Text *result){
    if (result == NULL){
        LOG_ERR("Received null pointer in readByteRangeToText");
        return 0;
    }
    
    /*
    * UTF8 CONVERSION START
    */
    uint32_t *codes = NULL;
    size_t i;
    size_t numChars = 0;
    size_t expectedLength = 0;
    size_t currChar = 0;
    
    //first, count the number of characters
    for (i = startByte; i <= endByte && data[i] != '\0'; i++){
        //check if the leading bits match what utf8 wants for the first byte
        //0xxxxxxx, 110xxxxx, 1110xxxx, 11110xxx
        //could of course replace with more bitwise operators over logical operators, but probably good enough
        //0xxxxxxx
        if (((~data[i]) & 128) == 128){
            numChars++;
            expectedLength += 1;

        //110xxxxx
        } else if ((data[i] & 192) == 192 && ((~data[i]) & 32) == 32) {
            numChars++;
            expectedLength += 2;

        //1110xxxx
        } else if ((data[i] & 224) == 224 && ((~data[i]) & 16) == 16) {
            numChars++;
            expectedLength += 3;

        //11110xxx
        } else if ((data[i] & 240) == 240 && ((~data[i]) & 8) == 8) {
            numChars++;
            expectedLength += 4;
        }
    }

    //if the expected number of bytes is incorrect, fail
    if (expectedLength != (endByte - startByte + 1)){
        LOG_ERR("Expected UTF8 length does not match actual length; expected %d bytes but had %d", expectedLength, (endByte - startByte + 1));
        return 0;
    }
        
    //allocate an array for the codes
    codes = calloc(numChars, sizeof(uint32_t));
    
    //reread the data and convert to utf8 code points in decimal
    currChar = 0;
    i = startByte;
    while (i <= endByte && data[i] != '\0'){
        //as before, could use bitwise operators over logical ones, but don't see need for such optimization yet
        //0xxxxxxx
        if (((~data[i]) & 128) == 128){
            codes[currChar] |= data[i]; //leading bit is 0, no need to mask
            currChar++;
            i++;

        //110xxxxx
        } else if ((data[i] & 192) == 192 && ((~data[i]) & 32) == 32) {
            // codes[currChar] = ((((uint32_t)data[i]) ^ 192) << 6) | (data[i + 1] ^ 128);
            codes[currChar] |= (((uint32_t)data[i]) ^ 192) << 6;
            codes[currChar] |= data[i + 1] ^ 128;
            currChar++;
            i += 2;

        //1110xxxx
        } else if ((data[i] & 224) == 224 && ((~data[i]) & 16) == 16) {
            // codes[currChar] = ((((uint32_t)data[i]) ^ 224) << 12) | ((((uint32_t)data[i+1]) ^ 128) << 6) | (((uint32_t)data[i+2]) ^ 128);
            codes[currChar] |= (((uint32_t)data[i]) ^ 224) << 12;
            codes[currChar] |= (((uint32_t)data[i+1]) ^ 128) << 6;
            codes[currChar] |= data[i+2] ^ 128;
            currChar++;
            i += 3;

        //11110xxx
        } else if ((data[i] & 240) == 240 && ((~data[i]) & 8) == 8) {
            // codes[currChar] = ((((uint32_t)data[i]) ^ 240) << 18) | ((((uint32_t)data[i+1]) ^ 128) << 12) | ((((uint32_t)data[i+2]) ^ 128) << 6) | (((uint32_t)data[i+3]) ^ 128);
            codes[currChar] |= (((uint32_t)data[i]) ^ 240) << 18;
            codes[currChar] |= (((uint32_t)data[i+1]) ^ 128) << 12;
            codes[currChar] |= (((uint32_t)data[i+2]) ^ 128) << 6;
            codes[currChar] |= data[i+3] ^ 128;
            currChar++;
            i += 4;
        
        //error
        } else {
            LOG_ERR("Something went horribly wrong parsing UTF8, leading bits don't make sense");
            free(codes);
            return 0;
        }
    }
    /*
    * UTF8 CONVERSION END
    */
    
    //store
    result->ids = codes;
    result->length = numChars;
    
    return 1;
}

void termText(){
    size_t i;
    for (i = 0; i < textTableLength; i++){
        if (textTable[i].length != 0){
            free(textTable[i].ids);
        }
    }
    free(textTable);
    textTableLength = 0;
}


