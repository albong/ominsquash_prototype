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

#define TEXTBOX_OFFSET 6
#define TEXTBOX_DRAW_SPEED 20

static Image *textboxImage; //replace with entity?
static unsigned totalDelta = 0;
Font *currentFont;
Text *currentText;
static int numLines = 0;
static int startCharacter = 0;//replace with an array for each line
static int numCharacters = 0;

void initTextbox(){
    textboxImage = loadImage("gfx/textbox.png");
    
    // currentText = readTextFromFile("data/text/en/00000.text", NULL);
    currentText = readTextFromFile("data/text/ko/00000.text", NULL);
    printf("%d\n", currentText->length);
    size_t i;
    for (i = 0; i < currentText->length; i++){
        printf("%u ", currentText->ids[i]);
    }
}

void termTextbox(){
    free_Image(textboxImage);
    
    //this stuff isn't finalized, and the text is freed elsewhere
}

int doTextbox(unsigned delta){
    totalDelta += delta;
    
    while (totalDelta > TEXTBOX_DRAW_SPEED){
        // numCharacters++;
        totalDelta -= TEXTBOX_DRAW_SPEED;
    }
    numCharacters = currentText->length;
    // printf("%d\n", getWidthOfText(currentFont, currentText, 0, numCharacters));
    
    return 0;
}

void drawTextbox(){
    size_t i;
    FontCharacter *fc;
    ImageRect src, dst;
    int cursorX, cursorY;
    int shiftX = (SCREEN_WIDTH / 2) - (textboxImage->width / 2);
    int shiftY = SCREEN_HEIGHT - 20 - textboxImage->height;
    
    //draw the box
    drawImage(textboxImage, shiftX, shiftY);
        
    //prep the destination line
    cursorX = shiftX + TEXTBOX_OFFSET;
    cursorY = shiftY + TEXTBOX_OFFSET;
        
    //draw the letters
    for (i = startCharacter; i < numCharacters; i++){
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

//takes a line string split at newlines
void addTextToTextbox(const char *text){

}

void configureTextboxForFont(Font *font){
    currentFont = font;
    
    numLines = (textboxImage->height - (2*TEXTBOX_OFFSET)) / font->lineHeight;
    printf("%d\n", numLines);
    
    //create and allocate an array of structs where each struct holds the starting character index and number of characters per line
    //one struct for each line
}