#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

Entity _player;

void initPlayer();
void doPlayer(int delta);
void drawPlayer();
void setPlayerTransitioning(int direction);
void stopPlayerTransitioning();
void movePlayer();

static void updatePlayerFrame(int delta);
static void updatePlayerOrientation();
static void updatePlayerPosition(int delta);

#endif

/*
Link's movement collision rectangle is actually about half size, so that it gives the
appearance of depth - that's why the top of his sprite slightly obscures things he stands
in front of.
*/
