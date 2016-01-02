#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "enemy.h"

// Structs
typedef struct Player {
    Entity e;
    int equippedAInd;
    int equippedBInd;
    int health;
    int milliHitstun;
} Player;

//Globals
Player _player;

// Loading
void initPlayer();

// Logic
void doPlayer(int delta);
void setPlayerTransitioning(int direction);
void stopPlayerTransitioning();
void movePlayer();
int playerTakeDamage(int amount);
static void updatePlayerFrame(int delta);
static void updatePlayerOrientation();
static void updatePlayerPosition(int delta);

// Drawing
void drawPlayer(); //we most likely don't really need this?  Maybe?  The player is prolly a special drawing case

#endif

/*
Link's movement collision rectangle is actually about half size, so that it gives the
appearance of depth - that's why the top of his sprite slightly obscures things he stands
in front of.
*/
