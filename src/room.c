#include "room.h"
#include "constants.h"

/////////////////////////////////////////////////
// Access
/////////////////////////////////////////////////
int getFlag(Room *room, int index, int flagNum){
    return (room->flags[index] >> flagNum) & 1;
}

void setFlag(Room *room, int index, int flagNum){
    room->flags[index] |= 1 << flagNum;
}

void clearFlag(Room *room, int index, int flagNum){
    room->flags[index] &= ~(1 << flagNum);
}

void toggleFlag(Room *room, int index, int flagNum){
    room->flags[index] ^= 1 << flagNum;
}

void generateWallList(Room *room, int tileSize){
    int i;
    int numWalls = 0;
    for (i = 0; i < ROOM_WIDTH * ROOM_HEIGHT; i++){
        if ((room->flags[i] >> ROOMF_IMPASSABLE) & 1){
            numWalls++;
        }
    }
    room->walls.numCircle = 0;
    room->walls.numRect = numWalls;
    room->walls.rects = malloc(sizeof(CollRect) * numWalls);
    
    int count = 0;
    for (i = 0; i < ROOM_WIDTH * ROOM_HEIGHT; i++){
        if ((room->flags[i] >> ROOMF_IMPASSABLE) & 1){
            room->walls.rects[count].x = (i % ROOM_WIDTH) * tileSize;
            room->walls.rects[count].y = (i / ROOM_WIDTH) * tileSize;
            room->walls.rects[count].w = tileSize;
            room->walls.rects[count].h = tileSize;
            count++;
        }
    }
}

void setEntityInitalPositions(Room *room){
    int i;
    for (i = 0; i < room->numEntities; i++){
        room->entities[i]->startX = room->entities[i]->x;
        room->entities[i]->startY = room->entities[i]->y;
    }
}

void setEnemyInitalPositions(Room *room){
    int i;
    for (i = 0; i < room->numEnemies; i++){
        room->enemies[i]->e.startX = room->enemies[i]->e.x;
        room->enemies[i]->e.startY = room->enemies[i]->e.y;
    }
}

void resetEntityPositions(Room *room){
    int i;
    for (i = 0; i < room->numEntities; i++){
        //no need to check if active?  probably won't matter
        room->entities[i]->x = room->entities[i]->startX;
        room->entities[i]->y = room->entities[i]->startY;
    }
}

void resetEnemyPositions(Room *room){
    int i;
    for (i = 0; i < room->numEnemies; i++){
        //no need to check if active?  probably won't matter
        room->enemies[i]->e.x = room->enemies[i]->e.startX;
        room->enemies[i]->e.y = room->enemies[i]->e.startY;
    }
}
