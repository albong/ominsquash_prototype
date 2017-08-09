#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "graphics.h"

void initTitleScreen();
void termTitleScreen();
int doTitleScreen(unsigned delta);//return code indicates what to do - start game, exit, load game?  load game seems difficult to do with just a return code
void drawTitleScreen();

#endif
