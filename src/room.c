#include "room.h"
#include "stair.h"
#include "constants.h"
#include "player.h"
#include "graphics.h"

/////////////////////////////////////////////////
// Loading/Unloading
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
    
    self->numStairs = 0;
    self->stairs = NULL;
    
    return self;
}

void term_Room(Room *self){
    size_t i;
    
    free(self->flags);
    free(self->tileIndices);
    free(self->transitionTiles);
    free(self->transitionToRoom);
    
    free_Image(self->buffer);
    
    term_Hitbox(&(self->walls));
    
    //shallow free, except for the animation - these will get freed for realsies in the creators, where the "master copy" is
    //using term_Entity here will free the sprite, but pointers will still exist to that memory in the master copy, so only shallow free
    for (i = 0; i < self->numEntities; i++){
        free(self->entities[i]->animation);
        self->entities[i]->animation = NULL;
        free(self->entities[i]);
        self->entities[i] = NULL;
    }
    free(self->entities);
    free(self->entityIds);
    free(self->entityInitialX);
    free(self->entityInitialY);
    self->entities = NULL;
    self->entityIds = NULL;
    self->entityInitialX = NULL;
    self->entityInitialY = NULL;
    self->numEntities = 0;
    
    //same as for entities - shallow free
    for (i = 0; i < self->numEnemies; i++){
        free(self->enemies[i]->e.animation);
        self->enemies[i]->e.animation = NULL;
        free(self->enemies[i]);
        self->enemies[i] = NULL;
    }
    free(self->enemies);
    free(self->enemyIds);
    free(self->enemyInitialX);
    free(self->enemyInitialY);
    self->enemies = NULL;
    self->enemyIds = NULL;
    self->enemyInitialX = NULL;
    self->enemyInitialY = NULL;
    self->numEnemies = 0;
    
    //doors - PIZZA
    
    //stairs
    for (i = 0; i < self->numStairs; i++){
        free(self->stairs[i]);
        self->stairs[i] = NULL;
    }
    free(self->stairs);
    self->stairs = NULL;
    self->numStairs = 0;
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
    for (j = 0; j < _player.e.hitboxes.movement[_player.e.animation->currFrame].numRect; j++){
        x1 = _player.e.x + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[j].x;
        x2 = x1 + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[j].w;
        y1 = _player.e.y + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[j].y;
        y2 = y1 + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[j].h;
        
        for (i = 0; i < self->numDoors; i++){
            //set door closed to enable correct hitboxes?
            setDoorOpen(self->doors[i], 0);
            
            //determine if door needs to be opened - assumes only 1 movement hitbox for now
            orientation = self->doors[i]->e.orientation;
            dx1 = self->doors[i]->e.x + self->doors[i]->e.hitboxes.movement[self->doors[i]->e.animation->currFrame].rects[0].x;
            dx2 = dx1 + self->doors[i]->e.hitboxes.movement[self->doors[i]->e.animation->currFrame].rects[0].w;
            dy1 = self->doors[i]->e.y + self->doors[i]->e.hitboxes.movement[self->doors[i]->e.animation->currFrame].rects[0].y;
            dy2 = dy1 + self->doors[i]->e.hitboxes.movement[self->doors[i]->e.animation->currFrame].rects[0].h;
            
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
