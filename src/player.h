#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct Player {
    Entity e;
    int equippedAInd;
    int equippedBInd;
} Player;

Player _player;

void initPlayer();
void doPlayer(int delta);
void drawPlayer();
void setPlayerTransitioning(int direction);
void stopPlayerTransitioning();
void movePlayer();
void playerCollideWithEnemy(Entity *enemy, int collCode);

static void updatePlayerFrame(int delta);
static void updatePlayerOrientation();
static void updatePlayerPosition(int delta);

#endif

/*
Link's movement collision rectangle is actually about half size, so that it gives the
appearance of depth - that's why the top of his sprite slightly obscures things he stands
in front of.
*/
