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
	Room *currentRoom;
    Room **roomList;
    char **entitySpriteNames;
    int *entitySpriteWidths;
    int *spriteIndices;
    int numEntitySprites;
} Area;

//globals
Area _current_area;

//functions
void doRoom(int delta);
void loadArea();
void drawCurrentRoom();
static int checkForRoomChange();
void changeRoom(int roomIndex, int direction, int delta);
static void drawRoomBuffers(Room *room);
HitBox getCurrentWalls();
static void loadEntitySprites();
static void drawRoomEntities(Room *room, double shiftX, double shiftY);
static void doRoomEntities();
int getNumRoomEntities();
Entity **getRoomEntityList();

static Room *createFirstDemoRoom();
static Room *createSecondDemoRoom();
static Room *createThirdDemoRoom();

#endif
