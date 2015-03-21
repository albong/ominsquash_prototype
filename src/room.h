#ifndef ROOM_H
#define ROOM_H

#include "stdint.h"
#include "SDL/SDL.h"

typedef struct Room {
    int width, height; //in tiles
    uint32_t *flags;
    //Enemy enemies[];
    int *tileIndices;
    int *transitionTiles;
    int *transitionToRoom;
    SDL_Surface *buffer;
    int connectingRooms[4]; //left right up down
} Room;

int getFlag(Room *room, int index, int flagNum);

/*
we need some sort of way to associate a tileset with a room - might make sense to
load the tileset for an area, and each room pulls from the area tiles.  So the outdoors
would be a single area (limited overworlds makes this feasible), each dungeon/interior
is an area.  Then at the transition screen we can load the resources.  An area holds
a 2d array of rooms, so when you leave a room, the area is responsible for drawing the
next room
*/

#endif

/*
To pull out a single flag from the value, do as follows:
Room r = ...;
unsigned flags = r.flags[x][y];
int flag_k = (flags >> k) & 1;

Flags are as follows (tentative list):
0  = is impassable
1  = is hole
2  = is shallow water
3  = is deep water
4  = is doorway (like link to the past, collide too much and go through, may need more
     flags for which direction is top of doorway)
5  = causes damage
6  = trigger event (could reuse the doorway direction flag - that flag could be
     indicating that when you leave the square in the flag's direction the action
     is triggered
7  = Whether or not to pull the player to the center as they walk over the tile, like
     what happens when you try to walk through a rounded doorway
8  = Animated tile
10 = UP
11 = DOWN
12 = LEFT
13 = RIGHT
*/
