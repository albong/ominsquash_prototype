#include "textbox.h"
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include <string.h>


#define NUM_CHAR_LINE 19
#define NUM_LINES 3
#define ALPHA_WIDTH 10
#define ALPHA_HEIGHT 10
#define LETTER_WIDTH 18
#define LETTER_HEIGHT 20

static Image *textboxImage;
static Image *alphabetSheet;

static int numLines;
static int currLine;
static char **lines;

static void cleanupText();

void initTextbox(){
    textboxImage = loadImage("gfx/textbox.png");
    alphabetSheet = loadImage("gfx/alphabet.png");
    numLines = 0;
    currLine = 0;
}

void termTextbox(){
    free_Image(textboxImage);
    free_Image(alphabetSheet);
    
    //this stuff isn't finalized, and the text is freed elsewhere
}

//return 0 for not finished, 1 if finished and frame should be popped
int doTextbox(unsigned delta){
    //use delta to make text not just appear but seemingly get typed - later
    //also use delta to make flashing cursor and whatnot
    if (numLines == 0){
        return 1;
    } else if (_input.x && !_inputRead.x){
        _inputRead.x = 1;
        
        if (numLines < currLine + NUM_LINES){
            cleanupText();
            return 1;
        } else {
            currLine += NUM_LINES;
        }
    }
    
    return 0;
}

void drawTextbox(){
    int i, j;
    int shiftX = (SCREEN_WIDTH / 2) - (textboxImage->width / 2);
    int shiftY = SCREEN_HEIGHT - 20 - textboxImage->height;
    int alphaX, alphaY;
    int charX, charY;
    int charNum;
    int OFFSET = 14;
    int LINE_SPACE = 6;
    
    drawImage(textboxImage, shiftX, shiftY);
    for (i = currLine; i < currLine + NUM_LINES && i < numLines; i++){
        for (j = 0; j < NUM_CHAR_LINE; j++){
            if (lines[i][j] == '\0'){
                break;
            }
            
            charNum = lines[i][j] - 32;
            if (charNum < 0 || charNum > 94){
                charNum = 99;
            }
            
            
            alphaX = ((lines[i][j] - 32) % ALPHA_WIDTH) * LETTER_WIDTH;
            alphaY = ((lines[i][j] - 32) / ALPHA_HEIGHT) * LETTER_HEIGHT;
            charX = shiftX + OFFSET + (j * LETTER_WIDTH);
            charY = shiftY + OFFSET + ((i - currLine) * (LINE_SPACE + LETTER_HEIGHT));

            ImageRect srcRect, dstRect;
            srcRect.x = alphaX;
            srcRect.y = alphaY;
            srcRect.w = LETTER_WIDTH;
            srcRect.h = LETTER_HEIGHT;
            
            dstRect.x = charX;
            dstRect.y = charY;
            dstRect.w = LETTER_WIDTH;
            dstRect.h = LETTER_HEIGHT;
            
            drawImageSrcDst(alphabetSheet, &srcRect, &dstRect);
        }
    }
}

//takes a line string split at newlines
void addTextToTextbox(const char *text){
    int i;
    int lineNum = 0;
    int charNum = 0;
    int length = strlen(text);
    
    numLines = 1;
    charNum = 0;
    for (i = 0; i < length; i++){
        if (text[i] == '\n'){
            charNum = 0;
            numLines++;
        } else {
            if (charNum + 1 == NUM_CHAR_LINE){
                numLines++;
                charNum = 0;
                //in case someone puts a newline on the character limit
                if (i + 1 < length && (text[i + 1] == '\n' || text[i + 1] == '\0')){
                    i++;
                }
            } else {
                charNum++;
            }
        }
    }
    // printf("%d\n", numLines);
    // numLines = 0;
    // return;
    
    //allocate
    lines = malloc(sizeof(char *) * numLines);
    for (i = 0; i < numLines; i++){
        //again, not sure where splittling line, the extra bytes are irrelevant
        lines[i] = malloc(sizeof(char) * NUM_CHAR_LINE);
    }
    
    //copy over the string - we're not sure about overrunning char limit so this is easiest
    charNum = 0;
    for (i = 0; i < length; i++){
        if (text[i] == '\n'){
            lines[lineNum][charNum] = '\0';
            lineNum++;
            charNum = 0;
        } else {
            lines[lineNum][charNum] = text[i];
            if (charNum + 1 == NUM_CHAR_LINE){
                lineNum++;
                charNum = 0;
                
                //in case someone puts a newline on the character limit
                if (i + 1 < length && text[i + 1] == '\n'){
                    i++;
                }
            } else {
                charNum++;
            }
        }
    }
    if (charNum + 1 != NUM_CHAR_LINE && lineNum < numLines){
        lines[lineNum][charNum] = '\0';
    }
}

// void testTextbox(){
    // int i;
    // int j;
    // for (i = currLine; i < numLines; i++){
        // for (j = 0; j < NUM_CHAR_LINE; j++){
            // if (lines[i][j] == '\0'){
                // break;
            // } else {
            // }
        // }
    // }
// }

void cleanupText(){
    int i;
    for (i = 0; i < numLines; i++){
        free(lines[i]);
        lines[i] = NULL;
    }
    free(lines);
    currLine = 0;
    numLines = 0;
}