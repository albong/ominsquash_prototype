#ifndef FONT_H
#define FONT_H
#include <stdint.h>
#include "graphics.h"

//PIZZA - need to correct the filenames in the font files from tga to png
//PIZZA - also really we could just have json data files but whatever

//this is based on the output of BMFont, from: http://www.angelcode.com/products/bmfont/
//I was forced to output 256x256 on account of the program crashing when trying to
//do 65536x65536; if you manage to work around this, change the sizes accordingly

//NOTE: These are like, packed, I guess?  If my arithmetic is right it doesn't really matter.  I tried for memory savings
//however, we probably could reduce all of the members of FontCharacter by a byte, since our font is so small
typedef struct FontCharacter {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t xOffset;
    int16_t yOffset;
    int16_t xAdvance;
    uint8_t sheetNum;
} FontCharacter;

typedef struct Font {
    Image **fontSheets;
    FontCharacter *characters; //array
    uint32_t *idTable; //sorted? use binary search to find?
    uint32_t numCharacters;
    uint16_t numSheets;
    uint16_t baseHeight;
    uint16_t lineHeight;
} Font;

Font *loadFontForLanguage(char *language);

/*

How to make sense for drawing:
(see http://www.angelcode.com/products/bmfont/doc/render_text.html)

 - lineHeight is how tall the lines should be
 - lines need a little padding on the end
 - given the id of a character to draw, find its FontCharacter;
    x, y, width, and height correspond to the rectangle about the
    character in the sheet given by sheetNum
    you draw that rectangle where your "cursor" is, offset by the
    given offsets (which may be negative)
    finally, you advance your "cursor" by advanceX, which is where
    you'll draw the next character
 - baseHeight is the height from the top of the line to the bottom
    of where letters are drawn, which in hindsight is basically useless

*/

#endif
