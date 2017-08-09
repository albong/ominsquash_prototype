#ifndef TITLE_H
#define TITLE_H

#include "graphics.h"

void initTitle();
void termTitle();
int doTitle(unsigned delta);//return code indicates what to do - start game, exit, load game?  load game seems difficult to do with just a return code
void drawTitle();

#endif
