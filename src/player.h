#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

Entity player;

void initPlayer();
void doPlayer(int delta);
void drawPlayer();

static void updatePlayerFrame(int delta);
static void updatePlayerOrientation();
static void updatePlayerPosition(int delta);
static void constrainPlayerPosition();

#endif

/*
Link's movement collision rectangle is actually about half size, so that it gives the
appearance of depth - that's why the top of his sprite slightly obscures things he stands
in front of.
*/
