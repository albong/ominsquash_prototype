#ifndef AREA_H
#define AREA_H

#include "SDL/SDL.h"

#include "graphics.h"
#include "room.h"

// Structs
typedef struct Tilesheet {
    SDL_Surface *sheet;
    int tileWidth, tileHeight;
} Tilesheet;

typedef struct Area {
    char *tilesetName;
    Tilesheet tilesheet;
    size_t numRooms;
	Room *currentRoom;
    Room **roomList;
    char **entitySpriteNames;
    int *entitySpriteWidths;
    Sprite **sprites;
    int numEntitySprites;
} Area;

// Globals
Area _current_area;

// Loading
void loadArea();
static void loadEntitySprites();
static void drawRoomBuffers(Room *room);

// Logic
void doRoom(int delta);
void changeRoom(int roomIndex, int direction, int delta);
void moveRoomEntities();
void moveRoomEnemies();
static int checkForRoomChange();
static void doRoomEntities();
static void doRoomEnemies();

// Drawing
void drawCurrentRoom();
static void drawRoomEntities(Room *room, double shiftX, double shiftY);
static void drawRoomEnemies(Room *room, double shiftX, double shiftY);

// Access
HitBox getCurrentWalls();
int getNumRoomEntities();
int getNumRoomEnemies();
Entity **getRoomEntityList();
Enemy **getRoomEnemyList();

// Demo
static Room *createFirstDemoRoom();
static Room *createSecondDemoRoom();
static Room *createThirdDemoRoom();

#endif
