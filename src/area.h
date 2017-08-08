#ifndef AREA_H
#define AREA_H

#include "graphics.h"
#include "room.h"
#include "constants.h"

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
    size_t numTemporaryEntities;
    Entity *temporaryEntities[NUM_AREA_TEMP_ENTITIES]; //array of pointers
    int id;
} Area;

// Globals
Area _current_area;

// Loading
Area *init_Area(Area *self);
int loadAreaById(int id);
//static void loadEntitySprites();

// Logic
void doRoom(int delta);
void moveRoomEntities();
void moveRoomEnemies();
void moveTemporaryEntities();

// Drawing
void drawCurrentRoom();
void drawCurrentRoomTopLayer();

// Access
Hitbox getCurrentWalls();
int getNumRoomEntities();
Entity **getRoomEntityList();
int getNumRoomEnemies();
Enemy **getRoomEnemyList();
size_t getNumRoomDoors();
Door **getRoomDoorList();
size_t getNumRoomStairs();
Stair **getRoomStairList();
void addTempEntityToArea(Entity *e);

#endif
