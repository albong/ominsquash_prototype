#ifndef AREA_H
#define AREA_H

#include "graphics.h"
#include "room.h"

// Structs
typedef struct Tilesheet {
    Image *sheet;
    int tileWidth, tileHeight;
} Tilesheet;

typedef struct Area {
    char *tilesetName;
    Tilesheet tilesheet;
    size_t numRooms;
	Room *currentRoom;
    Room **roomList;
//    char **entitySpriteNames;
//    int *entitySpriteWidths;
//    Sprite **sprites;
//    int numEntitySprites;
    int changingRooms;
} Area;

// Globals
Area _current_area;

// Loading
Area *init_Area(Area *self);
void loadArea();
//static void loadEntitySprites();
static void drawRoomBuffers(Room *room);

// Logic
void doRoom(int delta);
void changeRoom(int roomIndex, int direction, int delta);
void moveRoomEntities();
void moveRoomEnemies();
static int checkForRoomChange();
static void doRoomDoors(int delta);
static void doRoomEntities(int delta);
static void doRoomEnemies(int delta);

// Drawing
void drawCurrentRoom();
static void drawRoomDoors(Room *room, double shiftX, double shiftY);
static void drawRoomEntities(Room *room, double shiftX, double shiftY);
static void drawRoomEnemies(Room *room, double shiftX, double shiftY);

// Access
HitBox getCurrentWalls();
int getNumRoomEntities();
Entity **getRoomEntityList();
int getNumRoomEnemies();
Enemy **getRoomEnemyList();
size_t getNumRoomDoors();
Door **getRoomDoorList();

// Demo
static Room *createFirstDemoRoom();
static Room *createSecondDemoRoom();
static Room *createThirdDemoRoom();

#endif
