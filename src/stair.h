#ifndef STAIR_H
#define STAIR_H

#include "entity.h"

typedef struct Stair {
    Entity e;
    int sameArea;
    int toArea;
    int toRoom;
    int toX;
    int toY;
} Stair;

// Loading
Stair *init_Stair(Stair *self);

#endif