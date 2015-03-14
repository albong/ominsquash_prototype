#ifndef AREA_H
#define AREA_H

#include "SDL/SDL.h"

#include "graphics.h"
#include "room.h"

//structs
typedef struct Tilesheet {
    SDL_Surface *sheet;
    int tileWidth, tileHeight;
} Tilesheet;

typedef struct Area {
    char *tilesetName;
    Tilesheet tilesheet;
    int numRooms;
    int currentRoom;
    Room **roomList;
} Area;

//globals
Area _current_area;

//functions
void loadArea();
void drawCurrentRoom();

static void drawRoomBuffers(Room *room);

#endif
