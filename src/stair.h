#ifndef STAIR_H
#define STAIR_H

#include "entity.h"

typedef struct Stair {
    Entity e;
    int sameArea;
    int toArea;
    int toRoom;
    int toTileX;
    int toTileY;
} Stair;

// Loading
Stair *init_Stair(Stair *self);

#endif