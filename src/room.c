#include "room.h"
#include "constants.h"

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
    room->entityX = malloc(sizeof(double) * room->numEntities);
    room->entityY = malloc(sizeof(double) * room->numEntities);
    for (i = 0; i < room->numEntities; i++){
        room->entityX[i] = room->entities[i]->x;
        room->entityY[i] = room->entities[i]->y;
    }
}

void resetEntityPositions(Room *room){
    int i;
    for (i = 0; i < room->numEntities; i++){
        //no need to check if active?  probably won't matter
        room->entities[i]->x = room->entityX[i];
        room->entities[i]->y = room->entityY[i];
    }
}
