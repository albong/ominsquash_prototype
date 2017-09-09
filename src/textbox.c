#include "textbox.h"
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "data_reader.h"
#include "font.h"
#include <string.h>


#define NUM_CHAR_LINE 19
#define NUM_LINES 3
#define ALPHA_WIDTH 10
#define ALPHA_HEIGHT 10
#define LETTER_WIDTH 18
#define LETTER_HEIGHT 20

#define TEXTBOX_OFFSET 6 //the offset for the text area for the textbox; probably should be replaced by an arbitrary rectangle in a config file
#define TEXTBOX_DRAW_SPEED 20

#define LINEFEED_UTF8_ID 10
#define SPACE_UTF8_ID 32
#define DASH_UTF8_ID 45

static Image *textboxImage; //replace with entity?
static unsigned totalDelta = 0;
Font *currentFont;
Text *currentText;

//stuffed it into a struct because idk
//numCharacters is at most the number of characters in a line, which is reasonably small
typedef struct TextboxDisplay {
    int numLines;
    int currLine;
    size_t *startPosition; //array
    int *numCharacters; //array
    int *addDashAtLineEnd; //array
    int waitForPlayer;
    int maxWidth;
    int loadBlock;
    size_t blockStart;
} TextboxDisplay;

static TextboxDisplay display;

static void resetDisplay();
static void loadNextBlockOfText();
static int addCharacterToDisplay();//return 1 to indicate not to load further characters, 0 otherwise

void initTextbox(){
    textboxImage = loadImage("gfx/textbox.png");
    display.numLines = 0;
    display.currLine = 0;
    display.startPosition = NULL;
    display.numCharacters = NULL;
    display.addDashAtLineEnd = NULL;
    display.waitForPlayer = 0;
    display.loadBlock = 0;
    display.maxWidth = textboxImage->width - (2 * TEXTBOX_OFFSET);
}

void termTextbox(){
    free_Image(textboxImage);
    
    //this stuff isn't finalized, and the text is freed elsewhere
}

int doTextbox(unsigned delta){
    int result = 0;
    
    //we need to determine BEFORE display time what text is going to be displayed, so load up a block
    if (display.loadBlock){
        loadNextBlockOfText();
    }
    
    //for scrolling characters, currently unused
    totalDelta += delta;
    while (totalDelta > TEXTBOX_DRAW_SPEED && !display.waitForPlayer){
        totalDelta -= TEXTBOX_DRAW_SPEED;
    }
    
    //check if the player wants to advance text
    if (checkAndConsumeInput(X_BUTTON)){
        if (display.blockStart >= currentText->length){
            result = 1;
        } else {
            display.loadBlock = 1;
        }
    }
    
    return result;
}

void drawTextbox(){
    size_t i, j;
    FontCharacter *fc;
    ImageRect src, dst;
    int cursorX, cursorY;
    int shiftX = (SCREEN_WIDTH / 2) - (textboxImage->width / 2);
    int shiftY = SCREEN_HEIGHT - 20 - textboxImage->height;
    
    //draw the box
    drawImage(textboxImage, shiftX, shiftY);
    
    //loop over all lines of text
    for (j = 0; j < display.numLines; j++){
        //prep the destination line
        cursorX = shiftX + TEXTBOX_OFFSET;
        cursorY = shiftY + TEXTBOX_OFFSET + (j * currentFont->lineHeight);
            
        //draw the letters
        for (i = display.startPosition[j]; i < display.numCharacters[j]; i++){
            fc = findCharacter(currentFont, currentText->ids[i]);
            if (fc != NULL){
                //where is the character in the font sheet
                src.x = fc->x;
                src.y = fc->y;
                src.w = fc->width;
                src.h = fc->height;
                
                //adjust the destination accordingly
                dst.x = cursorX + fc->xOffset;
                dst.y = cursorY + fc->yOffset;
                dst.w = fc->width;
                dst.h = fc->height;
                
                //draw
                drawImageSrcDst(currentFont->fontSheets[fc->sheetNum], &src, &dst);
                
                //advance the cursor
                cursorX += fc->xAdvance;
            }
        }
    }
}

void setTextboxText(Text *text){
    currentText = text;
    resetDisplay();
    display.loadBlock = 1;
    display.blockStart = 0;
}

void configureTextboxForFont(Font *font){
    currentFont = font;
    
    //free our previous display data
    if (display.numLines != 0) {
        free(display.startPosition);
        free(display.numCharacters);
    }
    
    //allocate new data
    display.numLines = (textboxImage->height - (2*TEXTBOX_OFFSET)) / font->lineHeight;
    display.startPosition = malloc(sizeof(int) * display.numLines);
    display.numCharacters = malloc(sizeof(int) * display.numLines);
    display.addDashAtLineEnd = malloc(sizeof(int) * display.numLines);
    
    //initialize
    resetDisplay();
}

void resetDisplay(){
    size_t i;
    for (i = 0; i < display.numLines; i++){
        display.numCharacters[i] = 0;
        display.addDashAtLineEnd[i] = 0;
    }
    display.currLine = 0;
    display.waitForPlayer = 0;
}

void loadNextBlockOfText(){
    resetDisplay();
    display.startPosition[0] = display.blockStart;
    
    //holy crap what
    while(!addCharacterToDisplay()) {
    }
    
    //set the next load position - may have empty lines
    size_t i;
    for (i = 0; i < display.numLines; i++){
        if (display.numCharacters[i] != 0){
            display.blockStart = display.startPosition[i] + display.numCharacters[i] + 1;
        }
    }
    display.loadBlock = 0;
}

int addCharacterToDisplay(){
    size_t startPos = display.startPosition[display.currLine];
    int currLength = display.numCharacters[display.currLine];
    
    //first check if we've exhausted all text already
    if (startPos + currLength >= currentText->length){
        return 1;
    
    //check if the next character is a line break, in which case stop printing and wait, or increment what line we're on - linefeed won't get drawn
    } else if (currentText->ids[startPos + currLength + 1] == LINEFEED_UTF8_ID) {
        if (display.currLine == display.numLines - 1){
            display.numCharacters[display.currLine]++;
            return 1;
        } else {
            display.currLine++;
            display.startPosition[display.currLine] = startPos + currLength + 2; // 2 because skip the linefeed
            return 0;
        }
    }
    
    //we want to add a character to the current line, but we need to make sure we don't overrun the line
    //check if adding a new character makes the line too wide
    int result = 0;
    int lengthToSpace = -1;
    int i;
    int newWidth = getWidthOfText(currentFont, currentText, startPos, currLength + 1);
    if (newWidth > display.maxWidth){
        //walk backwards until we find a space character
        for (i = currLength + 1; i > 0; i--){
            if (currentText->ids[startPos + i] == SPACE_UTF8_ID){
                lengthToSpace = i;
                break;
            }
        }
        
        //if we found a space character, break the line there
        if (lengthToSpace != -1){
            display.numCharacters[display.currLine] = lengthToSpace;
            
            if (display.currLine == display.numLines -1){
                result = 1;
            } else {
                display.currLine++;
                display.startPosition[display.currLine] = startPos + lengthToSpace;
            }
            
        //if there was no space character, add a dash and advance the line without adding a new character
        } else {
            display.addDashAtLineEnd[display.currLine] = 1;
            
            if (display.currLine == display.numLines -1){
                result = 1;
            } else {
                display.currLine++;
                display.startPosition[display.currLine] = startPos + currLength;
            }
        }
        
    //if theres space on the current line for the next character
    } else {
        display.numCharacters[display.currLine]++;
    }
    
    return result;
}