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

// Loading/Unloading
Area *init_Area(Area *self);
int loadAreaById(int id);
void term_Area(Area *self);
void unloadCurrentArea();
void initAreaTransition();

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
int checkChangeArea();
int checkScreenWipe(double *x, double *y); //0 for none, 1 for inward, 2 for outward, set x and y to be center of wipe
void setWipeAfterLoadScreen();

#endif
