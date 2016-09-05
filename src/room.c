#include "room.h"
#include "constants.h"
#include "player.h"

/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
Room *init_Room(Room *self){
    if (self == NULL){
        return NULL;
    }
    
    self->flags = NULL;
    self->tileIndices = NULL;
    self->transitionTiles = NULL;
    self->transitionToRoom = NULL;
    self->buffer = NULL;
    self->bufferT = NULL;
    self->connectingRooms[0] = -1;
    self->connectingRooms[1] = -1;
    self->connectingRooms[2] = -1;
    self->connectingRooms[3] = -1;
    
    self->walls.numRect = 0;
    self->walls.numCircle = 0;
    self->walls.rects = NULL;
    self->walls.circles = NULL;
    
    self->numEntities = 0;
    self->entities = NULL;
    self->entityIds = NULL; //array
    self->entityInitialX = NULL; //array
    self->entityInitialY = NULL; //array
    
    self->numEnemies = 0;
    self->enemies = NULL;
    self->enemyIds = NULL; //array
    self->enemyInitialX = NULL; //array
    self->enemyInitialY = NULL; //array
    
    self->numDoors = 0;
    self->doors = NULL;
    
    return self;
}

void finalizeBuffer(Room *self){
    self->bufferT = convertToTexture(self->buffer);
}


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

void setEntityInitialPositions(Room *room){
    size_t i;
    for (i = 0; i < room->numEntities; i++){
        room->entities[i]->startX = room->entityInitialX[i];
        room->entities[i]->startY = room->entityInitialX[i];
    }
}

void setEnemyInitialPositions(Room *room){
    size_t i;
    for (i = 0; i < room->numEnemies; i++){
        room->enemies[i]->e.startX = room->enemyInitialX[i];
        room->enemies[i]->e.startY = room->enemyInitialY[i];
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

void clearAndResetEnemies(Room *room){
    size_t i;
    for (i = 0; i < room->numEnemies; i++){
        if (room->enemies[i]->e.active && room->enemies[i]->health <= 0){
            room->enemies[i]->e.active = 0;
        }
        /*
            Something something reset my health to default here
        */
    }
}

void setDoorStates(Room *self, int direction){
    double x1, x2, y1, y2, dx1, dx2, dy1, dy2;
    Orientation orientation;
    size_t i, j;
    for (j = 0; j < _player.e.moveHitBox[_player.e.currHitBox].numRect; j++){
        x1 = _player.e.x + _player.e.moveHitBox[_player.e.currHitBox].rects[j].x;
        x2 = x1 + _player.e.moveHitBox[_player.e.currHitBox].rects[j].w;
        y1 = _player.e.y + _player.e.moveHitBox[_player.e.currHitBox].rects[j].y;
        y2 = y1 + _player.e.moveHitBox[_player.e.currHitBox].rects[j].h;
        
        for (i = 0; i < self->numDoors; i++){
            //set door closed to enable correct hitboxes?
            setDoorOpen(self->doors[i], 0);
            
            //determine if door needs to be opened - assumes only 1 movement hitbox for now
            orientation = self->doors[i]->e.orientation;
            dx1 = self->doors[i]->e.x + self->doors[i]->e.moveHitBox[self->doors[i]->e.currHitBox].rects[0].x;
            dx2 = dx1 + self->doors[i]->e.moveHitBox[self->doors[i]->e.currHitBox].rects[0].w;
            dy1 = self->doors[i]->e.y + self->doors[i]->e.moveHitBox[self->doors[i]->e.currHitBox].rects[0].y;
            dy2 = dy1 + self->doors[i]->e.moveHitBox[self->doors[i]->e.currHitBox].rects[0].h;
            
            // r1.x+r1.w <= r2.x || r2.x+r2.w <= r1.x || r1.y+r1.h <= r2.y || r2.y+r2.h <= r1.y
            if ((((direction == ROOM_UP && orientation == DOWN) || (direction == ROOM_DOWN && orientation == UP)) && 
                    ((x1 <= dx1 && dx1 <= x2) || (dx1 <= x2 && x2 <= dx2))) 
                    ||
                    (((direction == ROOM_RIGHT && orientation == LEFT) || (direction == ROOM_LEFT && orientation == RIGHT)) && 
                    ((y1 <= dy1 && dy1 <= y2) || (dy1 <= y2 && y2 <= dy2)))
                    ){
                setDoorOpen(self->doors[i], 1);
            }
        }
    }
}
