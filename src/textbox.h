#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "font.h"

void initTextbox();
void termTextbox();
int doTextbox(unsigned delta);
void drawTextbox();
void addTextToTextbox(const char *text);
// void testTextbox();
void configureTextboxForFont(Font *font);

#endif

/*
    I dunno how this ought work yet - clearly the textbox needs to internally
    maintain a list of lines of text, with some way to have prompts between
    lines (so that you can have the infamous "do you understand" message).  It
    also needs to be "set", which I'm not sure how to say other than that whatever
    is adding the lines of text needs to tell the textbox that its done adding
    text, so that the textbox doesn't receive any new lines of text from another
    source until after it has finished displaying the current set.
*/

/*
    If characters are 18x20, then with 14px offsets from the sides, top, and bottom,
    and 6px space between each line, then we ought to be able to fit 19 characters 
    on a line, with 3 lines.

    This of course does not account for the scroll icon.  But if we're doing terminal
    style font then it can just flicker over the last letter of the line.

    The alphabet sheet has 10 rows of 10 characters each.  For now we'll just use
    ASCII, but we need to come up with some way to encode and translate text later.
    Also, we fully expect all text to be prepared so that it doesn't overrun the line.
    Probably whatever script/utility we come up with for encoding later should take
    all text, pad lines with spaces, and remove new lines, then we just read from 
    the buffer no sweat.

    Probs should've made the font width's odd numbers?
*/