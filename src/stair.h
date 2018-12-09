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
void free_Stair(Stair *self);

/*
 * TBH stairs and rooms should just be in area.h/c, but rooms are still in flux
 * so I don't want to do that since area changes a lot as is.
 */

#endif
