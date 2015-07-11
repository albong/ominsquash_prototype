#include <stdlib.h> 

#include "constants.h"
#include "area.h"
#include "room.h"
#include "graphics.h"
#include "player.h"
#include "entity.h"
#include "enemy.h"

#include "../entity/octorok.h"

#include "stdint.h"
#include "SDL/SDL.h"
 
static int changingRooms = 0;
static int changingToRoom = -1;
static unsigned totalDelta = 0;

typedef struct Transition{
    double oldX, oldY;
    double newX, newY;
    Room *newRoom;
    int direction;
} Transition;

Transition room_transition;

/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
void loadArea(){
    //load the tilesheet
    _current_area.tilesetName = "gfx/area1tiles.png";
    _current_area.tilesheet.sheet = loadImage(_current_area.tilesetName);
    _current_area.tilesheet.tileWidth = 16;
    _current_area.tilesheet.tileHeight = 16;
        
    //load the enemy sprites - do before rooms because rooms store enemies
    _current_area.numEntitySprites = 1;
    _current_area.entitySpriteNames = malloc(sizeof(char *) * 1);
    _current_area.entitySpriteNames[0] = malloc(sizeof(char) * (strlen("gfx/octoroksprite.png"))+1);
    strcpy(_current_area.entitySpriteNames[0], "gfx/octoroksprite.png");
    _current_area.entitySpriteWidths = malloc(sizeof(int) * 1);
    _current_area.entitySpriteWidths[0] = 18;
    loadEntitySprites();
    
    //load the rooms
    _current_area.numRooms = 1;
    _current_area.roomList = (Room**) malloc(sizeof(Room *) * _current_area.numRooms);
    _current_area.roomList[0] = createFirstDemoRoom();
    _current_area.roomList[1] = createSecondDemoRoom();
    _current_area.roomList[2] = createThirdDemoRoom();
    _current_area.currentRoom = _current_area.roomList[0];
}

static void loadEntitySprites(){
    _current_area.sprites = malloc(sizeof(Sprite *) * _current_area.numEntitySprites);
    int i;
    for (i = 0; i < _current_area.numEntitySprites; i++){
        _current_area.sprites[i] = loadAnimatedSprite(_current_area.entitySpriteNames[i], _current_area.entitySpriteWidths[i]);
    }
}

static void drawRoomBuffers(Room *room){
//    int currentRoom = _current_area.currentRoom;
//    Room *room = _current_area.roomList[currentRoom];
    int roomWidth = ROOM_WIDTH;
    int roomHeight = ROOM_HEIGHT;
    int tileWidth = _current_area.tilesheet.tileWidth;
    int tileHeight = _current_area.tilesheet.tileHeight;
    int numTilesWide = (_current_area.tilesheet.sheet->w - 1) / tileWidth;
    int numTilesHigh = (_current_area.tilesheet.sheet->h - 1) / tileHeight;
    SDL_Rect src, dst;
    int i, x, y;
    for (i = 0; i < roomWidth * roomHeight; i++){
        x = room->tileIndices[i] % numTilesWide;
        y = room->tileIndices[i] / numTilesWide;
        
        
        src.x = x * (tileWidth + 1) + 1;
        src.y = y * (tileHeight + 1) + 1;
        src.w = tileWidth;
        src.h = tileHeight;
    
        dst.x = (i % roomWidth) * tileWidth;
        dst.y = (i / roomWidth) * tileHeight;
        dst.w = tileWidth;
        dst.h = tileHeight;

//        drawImageSrcDst(_current_area.tilesheet.sheet, src, dst);
        SDL_BlitSurface(_current_area.tilesheet.sheet, &src, room->buffer, &dst);
    }
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void doRoom(int delta){
    totalDelta += delta;
    int newRoom;
    if (!changingRooms){
        doRoomEntities(delta);
        doRoomEnemies(delta);
        
        newRoom = checkForRoomChange();
        if (newRoom >= 0 && _current_area.currentRoom->connectingRooms[newRoom] != -1){
            totalDelta = 0;
            changingRooms = 1;
            changingToRoom = newRoom;
            changeRoom(_current_area.currentRoom->connectingRooms[changingToRoom], changingToRoom, delta);
            room_transition.newRoom = _current_area.roomList[_current_area.currentRoom->connectingRooms[newRoom]];
            resetEntityPositions(room_transition.newRoom);
            resetEnemyPositions(room_transition.newRoom);
            setPlayerTransitioning(changingToRoom);
        }
    } else {
        changeRoom(_current_area.currentRoom->connectingRooms[changingToRoom], changingToRoom, delta);
    }
}

void changeRoom(int roomIndex, int direction, int delta){
    double transPercent = totalDelta / MILLI_PER_TRANSITION;
    
    switch (direction){
        case ROOM_LEFT:
            room_transition.oldX = transPercent * SCREEN_WIDTH;
            room_transition.oldY = 0;
            room_transition.newX = (transPercent * SCREEN_WIDTH) - SCREEN_WIDTH;
            room_transition.newY = 0;
            break;
        case ROOM_RIGHT:
            room_transition.oldX = -transPercent * SCREEN_WIDTH;
            room_transition.oldY = 0;
            room_transition.newX = -(transPercent * SCREEN_WIDTH) + SCREEN_WIDTH;
            room_transition.newY = 0;
            break;
        case ROOM_UP:
            room_transition.oldX = 0;
            room_transition.oldY = transPercent * SCREEN_HEIGHT;
            room_transition.newX = 0;
            room_transition.newY = (transPercent * SCREEN_HEIGHT) - SCREEN_HEIGHT;
            break;
        case ROOM_DOWN:
            room_transition.oldX = 0;
            room_transition.oldY = -transPercent * SCREEN_HEIGHT;
            room_transition.newX = 0;
            room_transition.newY = -(transPercent * SCREEN_HEIGHT) + SCREEN_HEIGHT;
            break;
        default:
            break;
    }
    
    if (transPercent >= 1){
        changingRooms = 0;
        _current_area.currentRoom = _current_area.roomList[_current_area.currentRoom->connectingRooms[direction]];
        stopPlayerTransitioning();
    }
}

void moveRoomEntities(){
    int i;
    for (i = 0; i < _current_area.currentRoom->numEntities; i++){
        moveEntity(_current_area.currentRoom->entities[i]);
    }
}

void moveRoomEnemies(){
    int i;
    for (i = 0; i < _current_area.currentRoom->numEnemies; i++){
        moveEntity(&_current_area.currentRoom->enemies[i]->e);
    }
}

static int checkForRoomChange(){
    /*
            THIS NEEDS TO BE MADE MORE GENERAL
    */
    double left = _player.e.x + _player.e.moveHitBox[0].rects[0].x;
    double right = _player.e.x + _player.e.moveHitBox[0].rects[0].x + _player.e.moveHitBox[0].rects[0].w;
    double up = _player.e.y + _player.e.moveHitBox[0].rects[0].y;
    double down = _player.e.y + _player.e.moveHitBox[0].rects[0].y + _player.e.moveHitBox[0].rects[0].h;
    
    int roomRight = _current_area.tilesheet.tileWidth * ROOM_WIDTH;
    int roomDown = _current_area.tilesheet.tileHeight * ROOM_HEIGHT;

    if (left < 0){
        return ROOM_LEFT;
    } else if (right >roomRight){
        return ROOM_RIGHT;
    } else if (up < 0){
        return ROOM_UP;
    } else if (down > roomDown){
        return ROOM_DOWN;
    } else {
        return -1;
    }
}

static void doRoomEntities(int delta){
    int i;
    Entity *self;
    
    //loop through all entities and perform their action
    for (i = 0; i < _current_area.currentRoom->numEntities; i++){
        self = _current_area.currentRoom->entities[i];
        if (self->active == 1 && self->action){
            self->action(self, delta);
        }
    }
}

static void doRoomEnemies(int delta){
    int i;
    Enemy *self;
    
    //loop through all entities and perform their action
    for (i = 0; i < _current_area.currentRoom->numEnemies; i++){
        self = _current_area.currentRoom->enemies[i];
        if (self->e.active == 1 && self->action){
            self->action(self, delta);
        }
    }
}


/////////////////////////////////////////////////
// Drawing
/////////////////////////////////////////////////
void drawCurrentRoom(){
    if (!changingRooms){
        drawImage(_current_area.currentRoom->buffer, 0, 0);
        drawRoomEntities(_current_area.currentRoom, 0, 0);
        drawRoomEnemies(_current_area.currentRoom, 0, 0);
    } else {
        drawImage(_current_area.currentRoom->buffer, room_transition.oldX, room_transition.oldY);
        drawImage(room_transition.newRoom->buffer, room_transition.newX, room_transition.newY);
        
        drawRoomEntities(_current_area.currentRoom, room_transition.oldX, room_transition.oldY);
        drawRoomEntities(room_transition.newRoom, room_transition.newX, room_transition.newY);
        
        drawRoomEnemies(_current_area.currentRoom, room_transition.oldX, room_transition.oldY);
        drawRoomEnemies(room_transition.newRoom, room_transition.newX, room_transition.newY);
    }
}

static void drawRoomEntities(Room *room, double shiftX, double shiftY){
    int i;
    for (i = 0; i < room->numEntities; i++){
        if (room->entities[i]->active){
//            drawAnimatedSprite(room->entities[i]->sprite, 0, room->entities[i]->x + shiftX, room->entities[i]->y + shiftY);
            room->entities[i]->draw(room->entities[i], shiftX, shiftY);
        }
    }
}

static void drawRoomEnemies(Room *room, double shiftX, double shiftY){
    int i;
    for (i = 0; i < room->numEnemies; i++){
        if (room->enemies[i]->e.active){
//            drawAnimatedSprite(room->entities[i]->sprite, 0, room->entities[i]->x + shiftX, room->entities[i]->y + shiftY);
            room->enemies[i]->e.draw(&room->enemies[i]->e, shiftX, shiftY);
        }
    }
}


/////////////////////////////////////////////////
// Access Functions
/////////////////////////////////////////////////
HitBox getCurrentWalls(){
    return _current_area.currentRoom->walls;
}

int getNumRoomEntities(){
    return _current_area.currentRoom->numEntities;
}

int getNumRoomEnemies(){
    return _current_area.currentRoom->numEnemies;
}

Entity **getRoomEntityList(){
    return _current_area.currentRoom->entities;
}

Enemy **getRoomEnemyList(){
    return _current_area.currentRoom->enemies;
}


/////////////////////////////////////////////////
// Demo
/////////////////////////////////////////////////
static Room *createFirstDemoRoom(){
    Room *firstRoom = malloc(sizeof(Room));
    firstRoom->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
    firstRoom->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
    int i;
    for (i = 0; i < ROOM_HEIGHT * ROOM_WIDTH; i++){
        firstRoom->tileIndices[i] = 41;
        firstRoom->flags[i] = 0;
    }
    firstRoom->connectingRooms[0] = 1;
    firstRoom->connectingRooms[1] = 1;
    firstRoom->connectingRooms[2] = 1;
    firstRoom->connectingRooms[3] = 1;
    firstRoom->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    drawRoomBuffers(firstRoom);
    generateWallList(firstRoom, _current_area.tilesheet.tileWidth);
    
    firstRoom->numEntities = 0;
    firstRoom->numEnemies = 1;
    firstRoom->enemies = malloc(sizeof(Enemy) * firstRoom->numEnemies);
    firstRoom->enemies[0] = createOctorok(_current_area.sprites[0]);
    setEntityInitalPositions(firstRoom);
    setEnemyInitalPositions(firstRoom);
    
    return firstRoom;
}

static Room *createSecondDemoRoom(){
    Room *room = malloc(sizeof(Room));
    room->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
    room->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
    int i;
    for (i = 0; i < ROOM_HEIGHT * ROOM_WIDTH; i++){
        room->tileIndices[i] = 20;
        room->flags[i] = 0;
    }
    
    room->tileIndices[ROOM_WIDTH*3 + 5] = 7;
    room->tileIndices[ROOM_WIDTH*3 + 6] = 9;
    room->tileIndices[ROOM_WIDTH*4 + 5] = 37;
    room->tileIndices[ROOM_WIDTH*4 + 6] = 39;
    setFlag(room, ROOM_WIDTH*3 + 5, ROOMF_IMPASSABLE);
    setFlag(room, ROOM_WIDTH*3 + 6, ROOMF_IMPASSABLE);
    setFlag(room, ROOM_WIDTH*4 + 5, ROOMF_IMPASSABLE);
    setFlag(room, ROOM_WIDTH*4 + 6, ROOMF_IMPASSABLE);
    
    room->connectingRooms[0] = 0;
    room->connectingRooms[1] = 0;
    room->connectingRooms[2] = 0;
    room->connectingRooms[3] = 0;
    room->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    drawRoomBuffers(room);
    generateWallList(room, _current_area.tilesheet.tileWidth);

    room->numEntities = 0;

    return room;
}

static Room *createThirdDemoRoom(){
    //61 & 63
    Room *room = malloc(sizeof(Room));
    room->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
    room->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
    int i;
    for (i = 0; i < ROOM_HEIGHT * ROOM_WIDTH; i++){
        if (i % ROOM_WIDTH == 0){
            room->tileIndices[i] = 19;
            setFlag(room, i, ROOMF_IMPASSABLE);
        } else if (i % ROOM_WIDTH == ROOM_WIDTH-1){
            room->tileIndices[i] = 21;
            setFlag(room, i, ROOMF_IMPASSABLE);
        } else if (i / ROOM_WIDTH == 0){
            room->tileIndices[i] = 5;
            setFlag(room, i, ROOMF_IMPASSABLE);
        } else if (i / ROOM_WIDTH == ROOM_HEIGHT-1){
            room->tileIndices[i] = 35;
            setFlag(room, i, ROOMF_IMPASSABLE);
        } else {
            room->tileIndices[i] = 20;
            room->flags[i] = 0;
        }
    }
    
    room->tileIndices[0] = 4;
    room->tileIndices[ROOM_WIDTH-1] = 6;
    room->tileIndices[(ROOM_HEIGHT-1)*ROOM_WIDTH] = 34;
    room->tileIndices[ROOM_HEIGHT*ROOM_WIDTH - 1] = 36;
    
    room->tileIndices[ROOM_WIDTH/2] = 20;
    clearFlag(room, ROOM_WIDTH/2, ROOMF_IMPASSABLE);

    room->connectingRooms[0] = -1;
    room->connectingRooms[1] = -1;
    room->connectingRooms[2] = -1;
    room->connectingRooms[3] = -1;
    room->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    generateWallList(room, _current_area.tilesheet.tileWidth);
    drawRoomBuffers(room);
    
    room->numEntities = 0;
    
    return room;
}
