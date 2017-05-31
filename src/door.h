#ifndef DOOR_H
#define DOOR_H

#include "entity.h"
#include "graphics.h"

typedef struct Door {
    Entity e;
    int isOpen;
    int changingState;
    int isLocked;
} Door;

// PIZZA - graphics stuff needs updating
// Door *createDoor(Sprite *sprite, Orientation direction, double x, double y); //replace id with sprite - have areas have list of door sprites
Door *createDoor(Orientation direction, double x, double y); //replace id with sprite - have areas have list of door sprites
void setDoorOpening(Door *self, int open);
void setDoorOpen(Door *self, int open);
void setDoorLocked(Door *self, int locked);


/*
SEE BELOW FIRST
We should give doors an id number to determine their sprite, and an orientation to determine what frames and what hitboxes will be used
Do we want to give doors their own directory and have different classes of doors?  That may be a good idea for handling keys and
any special logic.  But then again, we can probably handle that by adding a collision check and something to the struct, then
checking the player's inventory.

We can probably name the sprites by their id (so door1.png and etc)?

Maybe we need to make a "loader" directory where we store the various loader and constant files for the different things we 
want to programmatically load.
*/

/*

Idea is, interaction hitboxes are for doors opening, or for unlocking doors; movement hitboxes are for blocking the player

Second idea is, doors are always closed/locked, only open if unlocked and player touches interact hitbox; locked door requires
that player press interaction button in interaction hitbox, as opposed to walking into the door?

*/

#endif
