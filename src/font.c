#include "font.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"
#include "graphics.h"
#include "constants.h"
#include "logging.h"

//note: I had to change the colors of the output, had to get that sweet magic pink
//
//I also expect to be reading a binary file, in the correct format, as specified on the BMFont site
//
//if you're wondering why I didn't just read into structs and such, its because idgaf
//
//I think the computer I generated the original fnt file on is little endian, if you generate
//a big endian file this is all boned - again, idgaf, we'll burn that bridge when we cross it

static void readCommonBlock(uint8_t *block, uint32_t blockSize, Font *font);
static void readPagesBlock(uint8_t *block, uint32_t blockSize, Font *font);
static void readCharsBlock(uint8_t *block, uint32_t blockSize, Font *font);
static uint16_t readUInt16FromBlock(uint8_t *block);
static uint16_t readInt16FromBlock(uint8_t *block);
static uint32_t readUInt32FromBlock(uint8_t *block);

Font *loadFontForLanguage(char *language){
    char filename[80];
    uint8_t *data;
    size_t index;
    char blockType;
    uint32_t blockSize;
    uint8_t *block;
    unsigned long fileSize;
    
    //null check
    if (language == NULL || strlen(language) != 2){
        return NULL;
    }
    
    //read in the file
    sprintf(filename, "data/fonts/%s.fnt", language);
    data = readBinaryFileToCharStar(filename, &fileSize);  //PIZZA type mismatch possible here
    
    //null check, size check, header check
    //this magic number is the bare minimum number of bytes to produce a valid Font struct
    if (data == NULL || fileSize < (4 + 5*4 + 15 + 15 + 1 + 20) || (data[0] != 66 && data[0] != 77 && data[0] != 70 && data[0] != 3)){ 
        LOG_ERR("Problem with font file %s", filename);
        return NULL;
    }
    Font *font = malloc(sizeof(Font));
    
    //loop over all the data
    index = 4;
    while (index < fileSize){
        //read int the block header
        blockType = data[index];
        blockSize = readUInt32FromBlock(data + index + 1);
        
        //"copy" the block
        block = data + index + 5;//five bytes for the header
        
        //handle the block
        switch (blockType) {
            case 2:
                readCommonBlock(block, blockSize, font);
                break;
            case 3:
                readPagesBlock(block, blockSize, font);
                break;
            case 4:
                readCharsBlock(block, blockSize, font);
                break;
            default:
                //case 1 would be the info block, which has nothing of value for us
                //case 5 would be kerning pairs, which we ignore
                break;
        }
        
        //increment
        index += blockSize + 5; //five bytes for the header
    }
    
    //free the file's data
    free(data);
    
    //return the result
    LOG_INF("Font at %s loaded", filename);
    return font;
}

void readCommonBlock(uint8_t *block, uint32_t blockSize, Font *font){
    size_t index = 0;
    
    //first two bytes are line height
    font->lineHeight = readUInt16FromBlock(block + index);
    index += 2;
    
    //next two bytes are the baseHeight
    font->baseHeight = readUInt16FromBlock(block + index);
    index += 2;
    
    //skip ahead 4 bytes to the number of tilesheets (pages in the naming of BMFont)
    index += 4;
    font->numSheets = readUInt16FromBlock(block + index);
    
    //I don't care about anything else in this block, but there is more
}

void readPagesBlock(uint8_t *block, uint32_t blockSize, Font *font){
    //I guess in theory, the blocks might be out of order?  in which case we might not know how many tilesheets there are at this point
    //Its unclear, and maybe we ought to handle it?
    //
    //Since the filenames are null separated, can just pass the whole block as a string
    size_t i;
    char filename[FILENAME_BUFFER_SIZE];
    
    //check if the sizes are the same
    if (blockSize / (strlen(block)+1) != font->numSheets){
        LOG_ERR("Font file corrupted OR font file blocks are out of order");
    }
    
    //allocate the array
    font->fontSheets = malloc(sizeof(Image *) * font->numSheets);
    
    //go over each sheet and load the image
    for (i = 0; i < font->numSheets; i++){
        sprintf(filename, "gfx/fonts/%s", block);
        font->fontSheets[i] = loadImage(filename);
        block += strlen(block)+1;
        LOG_INF("Font image at %s loaded", filename);
    }
}

void readCharsBlock(uint8_t *block, uint32_t blockSize, Font *font){
    //i realize now these ought have switched names
    size_t index = 0;
    uint32_t position = 0;
    
    //get the number of characters and allocate
    font->numCharacters = blockSize / 20; //each field is 20 bytes
    font->characters = malloc(sizeof(FontCharacter) * font->numCharacters);
    font->idTable = malloc(sizeof(uint32_t) * font->numCharacters); //yes, I sizeof'd
    
    //loop over the block
    while (index < blockSize && position < font->numCharacters){
        //first four bytes are an id
        font->idTable[position] = readUInt32FromBlock(block + index);
        index += 4;
        
        //next two bytes are x, and the two after are y
        font->characters[position].x = readUInt16FromBlock(block + index);
        index += 2;
        font->characters[position].y = readUInt16FromBlock(block + index);
        index += 2;
        
        //the next two pairs are width and height
        font->characters[position].width = readUInt16FromBlock(block + index);
        index += 2;
        font->characters[position].height = readUInt16FromBlock(block + index);
        index += 2;
        
        //then xoffset and yoffset
        font->characters[position].xOffset = readInt16FromBlock(block + index);
        index += 2;
        font->characters[position].yOffset = readInt16FromBlock(block + index);
        index += 2;
        
        //xadvance is 2 bytes
        font->characters[position].xAdvance = readInt16FromBlock(block + index);
        index += 2;
        
        //the page (tilesheet number) is only 1 byte
        font->characters[position].sheetNum = block[index];
        index += 1;
        
        //and we don't care about the channel
        index +=1;
        
        //increment
        position++;
    }
}

uint16_t readUInt16FromBlock(uint8_t *block){
    return ((uint16_t)block[0+1]<<8) | ((uint16_t)block[0]);
}

uint16_t readInt16FromBlock(uint8_t *block){
    return (int16_t)(((uint16_t)block[0+1]<<8) | ((uint16_t)block[0]));
}

uint32_t readUInt32FromBlock(uint8_t *block){
    return ((uint32_t)block[0+3]<<24) | ((uint32_t)block[0+2]<<16) | ((uint32_t)block[0+1]<<8) | ((uint32_t)block[0]);
}

int getWidthOfText(Font *font, Text *text, int startIndex, int length){
    size_t i;
    int result = 0;
    FontCharacter *c;
    
    //null check
    if (text == NULL || startIndex >= text->length){
        return 0;
    }
    
    //add up the lengths
    for (i = startIndex; (i - startIndex) < length && i < text->length; i++){
        c = findCharacter(font, text->ids[i]);
        if (c != NULL){
            //take the max of the sizes
            // if (c->xAdvance > c->width + c->xOffset){
                result += c->xAdvance;
            // } else {
                // result += c->width + c->xOffset;
            // }
        }
    }
    
    return result;
}

FontCharacter *findCharacter(Font *font, uint32_t id){
    if (font == NULL){
        return NULL;
    }
    
    //straight search - hella inefficient
    FontCharacter *result = NULL;
    size_t i;
    for (i = 0; i < font->numCharacters; i++){
        if (font->idTable[i] == id){
            result = font->characters + i;
            break;
        }
    }
    
    return result;
}

void free_Font(Font *self){
    if (self == NULL){
        LOG_WAR("Tried to free NULL font");
        return;
    }
    
    size_t i;
    for (i = 0; i < self->numSheets; i++){
        free_Image(self->fontSheets[i]);
        self->fontSheets[i] = NULL;
    }
    free(self->fontSheets);
    
    free(self->characters);
    free(self->idTable);
    
    self->numCharacters = 0;
    self->numSheets = 0;
    self->baseHeight = 0;
    self->lineHeight = 0;
    
    free(self);
    LOG_INF("Font at %p freed", self);
}
