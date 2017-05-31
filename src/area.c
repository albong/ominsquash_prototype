#include <stdlib.h> 
#include <stdio.h> 

#include "constants.h"
#include "area.h"
#include "room.h"
#include "graphics.h"
#include "player.h"
#include "entity.h"
#include "enemy.h"

#include "data_reader.h"
#include "entity_creator.h"
#include "enemy_creator.h"

#include "stdint.h"
 
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

static void loadAreaEnemyData(Area *self);
static void createAreaEnemies(Area *self);
static void loadAreaEntityData(Area *self);
static void createAreaEntities(Area *self);

/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
Area *init_Area(Area *self){
    if (self == NULL){
        return NULL;
    }

    self->tilesetName = NULL;

    self->tilesheet.sheet = NULL;
    self->tilesheet.tileWidth = 0; 
    self->tilesheet.tileHeight = 0;

    self->numRooms = 0;
	self->currentRoom = NULL;
    self->roomList = NULL;
    self->changingRooms = 0;
    
    return self;
}

void loadArea(){
    size_t i;

    //remove the janky pointer bidness later so we can cache areas
    readAreaFromFile("data/areas/0.area", &_current_area);
    
    //load the tilesheet
    // _current_area.tilesetName = "gfx/area1tiles.png";
    _current_area.tilesheet.sheet = loadImage(_current_area.tilesetName);
    _current_area.tilesheet.tileWidth = TILE_SIZE;
    _current_area.tilesheet.tileHeight = TILE_SIZE;

    //draw the buffers
    for (i = 0; i < _current_area.numRooms; i++){
        _current_area.roomList[i]->buffer = getEmptyImage(TILE_SIZE * ROOM_WIDTH, TILE_SIZE * ROOM_HEIGHT);
        drawRoomBuffers(_current_area.roomList[i]);
    }
    
    //PIZZA - enemy loading not written yet since enemy loading not overhauled
    Room *TEMP_ROOM = _current_area.roomList[1];
    TEMP_ROOM->numEnemies = 1;
//    TEMP_ROOM->enemies = malloc(sizeof(Enemy) * TEMP_ROOM->numEnemies);
    TEMP_ROOM->enemyIds = malloc(sizeof(size_t) * TEMP_ROOM->numEnemies);
    TEMP_ROOM->enemyInitialX = malloc(sizeof(double) * TEMP_ROOM->numEnemies);
    TEMP_ROOM->enemyInitialY = malloc(sizeof(double) * TEMP_ROOM->numEnemies);
//    TEMP_ROOM->enemies[0] = createOctorok(_current_area.sprites[0]);
//    TEMP_ROOM->enemies[0] = createOctorok();
    TEMP_ROOM->enemyIds[0] = 0;
    TEMP_ROOM->enemyInitialX[0] = 1;
    TEMP_ROOM->enemyInitialY[0] = 1;
    
    //load the enemies for all of the rooms, set their positions
    // loadAreaEnemySprites(&_current_area);
    loadAreaEnemyData(&_current_area);
    loadAreaEntityData(&_current_area);
    createAreaEnemies(&_current_area);
    createAreaEntities(&_current_area);

    //PIZZA - testing stuff to set the room we're going to use, add starting room stuff later
    _current_area.currentRoom = _current_area.roomList[0];
    
    //PIZZA remove this later
    //PIZZA - doors need to be reworked, but they're using the old graphics code
    TEMP_ROOM = _current_area.roomList[0];
    // Sprite *doorSprite = loadAnimatedSprite("gfx/door1.png", TILE_SIZE);
    TEMP_ROOM->numDoors = 0;
    // TEMP_ROOM->doors = malloc(sizeof(Door *) * TEMP_ROOM->numDoors);
    // TEMP_ROOM->doors[0] = createDoor(doorSprite, UP, (ROOM_WIDTH/2) * TILE_SIZE, 0);
    // TEMP_ROOM->doors[1] = createDoor(doorSprite, DOWN, (ROOM_WIDTH/2) * TILE_SIZE, (ROOM_HEIGHT-1) * TILE_SIZE);
    
    //reset enemy and entity placement
    for (i = 0; i < _current_area.numRooms; i++){
        resetEnemyPositions(_current_area.roomList[i]);
        resetEntityPositions(_current_area.roomList[i]);
    }
    
    _current_area.changingRooms = 0;
}

void loadAreaEnemyData(Area *self){
    /*
    
    When we move to loading in files, I would want the area file to have the list of unique ids already in the file
    
    */
    size_t count = 0;
    size_t *ids = NULL;
    int matched = 0;
    size_t i, j, k;
    for (i = 0; i < self->numRooms; i++){
        for (j = 0; j < self->roomList[i]->numEnemies; j++){
            for (k = 0; k < count; k++){
                if (self->roomList[i]->enemyIds[j] == ids[k]){
                    matched = 1;
                    break;
                }
            }
            if (matched != 1){
                count++;
                ids = realloc(ids, sizeof(size_t) * count);
                ids[count-1] = self->roomList[i]->enemyIds[j];
            }
            matched = 0;
        }
    }
    
    // loadEnemySprites(ids, count);
    loadEnemyData(ids, count);
    free(ids);
}

void createAreaEnemies(Area *self){
    size_t i, j;
    Room *currRoom;
    for (i = 0; i < self->numRooms; i++){
        currRoom = self->roomList[i];
        currRoom->enemies = malloc(sizeof(Enemy *) * currRoom->numEnemies);
        for (j = 0; j < currRoom->numEnemies; j++){
            currRoom->enemies[j] = createEnemyById(currRoom->enemyIds[j]);
        }
        setEnemyInitialPositions(currRoom);
    }
}


void loadAreaEntityData(Area *self){
    /*
    
    When we move to loading in files, I would want the area file to have the list of unique ids already in the file
    
    */
    size_t count = 0;
    size_t *ids = NULL;
    int matched = 0;
    size_t i, j, k;
    for (i = 0; i < self->numRooms; i++){
        for (j = 0; j < self->roomList[i]->numEntities; j++){
            for (k = 0; k < count; k++){
                if (self->roomList[i]->entityIds[j] == ids[k]){
                    matched = 1;
                    break;
                }
            }
            if (matched != 1){
                count++;
                ids = realloc(ids, sizeof(size_t) * count);
                ids[count-1] = self->roomList[i]->entityIds[j];
            }
            matched = 0;
        }
    }
    
    // loadEntitySprites(ids, count);
    loadEntityData(ids, count);
    free(ids);
}

void createAreaEntities(Area *self){
    size_t i, j;
    Room *currRoom;
    for (i = 0; i < self->numRooms; i++){
        currRoom = self->roomList[i];
        currRoom->entities = malloc(sizeof(Entity *) * currRoom->numEntities);
        for (j = 0; j < currRoom->numEntities; j++){
            currRoom->entities[j] = createEntityById(currRoom->entityIds[j]);
        }
        setEntityInitialPositions(currRoom);
    }
}

static void drawRoomBuffers(Room *room){
//    int currentRoom = _current_area.currentRoom;
//    Room *room = _current_area.roomList[currentRoom];
    int roomWidth = ROOM_WIDTH;
    int roomHeight = ROOM_HEIGHT;
    int tileWidth = _current_area.tilesheet.tileWidth;
    int tileHeight = _current_area.tilesheet.tileHeight;
    int numTilesWide = (_current_area.tilesheet.sheet->width - 1) / tileWidth;
    int numTilesHigh = (_current_area.tilesheet.sheet->height - 1) / tileHeight;
    SDL_Rect src, dst;
    ImageRect srcR, dstR;
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
        
        srcR.x = x * (tileWidth + 1) + 1;
        srcR.y = y * (tileHeight + 1) + 1;
        srcR.w = tileWidth;
        srcR.h = tileHeight;
    
        dstR.x = (i % roomWidth) * tileWidth;
        dstR.y = (i / roomWidth) * tileHeight;
        dstR.w = tileWidth;
        dstR.h = tileHeight;

        drawImageToImage(_current_area.tilesheet.sheet, room->buffer, &srcR, &dstR);
    }
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void doRoom(int delta){
    totalDelta += delta;
    int newRoom;
    if (!changingRooms){
        doRoomDoors(delta);
        doRoomEntities(delta);
        doRoomEnemies(delta);
        
        newRoom = checkForRoomChange();
        if (newRoom >= 0 && _current_area.currentRoom->connectingRooms[newRoom] != -1){
            totalDelta = 0;
            changingRooms = 1;
            _current_area.changingRooms = 1;
            changingToRoom = newRoom;
            room_transition.newRoom = _current_area.roomList[_current_area.currentRoom->connectingRooms[newRoom]];
            resetEntityPositions(room_transition.newRoom);
            resetEnemyPositions(room_transition.newRoom);
            clearAndResetEnemies(room_transition.newRoom);
            setDoorStates(room_transition.newRoom, changingToRoom);
            setPlayerTransitioning(changingToRoom);
            changeRoom(_current_area.currentRoom->connectingRooms[changingToRoom], changingToRoom, delta);
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
        _current_area.changingRooms = 0;
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

void doRoomDoors(int delta){
    size_t i;
    Door *self;
    
    for (i = 0; i < _current_area.currentRoom->numDoors; i++){
        self = _current_area.currentRoom->doors[i];
        if (self->e.active == 1 && self->e.action){
            self->e.action(self, delta);
        }
    }
}

static void doRoomEntities(int delta){
    size_t i;
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
        drawRoomDoors(_current_area.currentRoom, 0, 0);
        drawRoomEntities(_current_area.currentRoom, 0, 0);
        drawRoomEnemies(_current_area.currentRoom, 0, 0);
    } else {
        drawImage(_current_area.currentRoom->buffer, room_transition.oldX, room_transition.oldY);
        drawImage(room_transition.newRoom->buffer, room_transition.newX, room_transition.newY);
        
        drawRoomDoors(_current_area.currentRoom, room_transition.oldX, room_transition.oldY);
        drawRoomDoors(room_transition.newRoom, room_transition.newX, room_transition.newY);
        
        drawRoomEntities(_current_area.currentRoom, room_transition.oldX, room_transition.oldY);
        drawRoomEntities(room_transition.newRoom, room_transition.newX, room_transition.newY);
        
        drawRoomEnemies(_current_area.currentRoom, room_transition.oldX, room_transition.oldY);
        drawRoomEnemies(room_transition.newRoom, room_transition.newX, room_transition.newY);
    }
}

static void drawRoomDoors(Room *room, double shiftX, double shiftY){
    size_t i;
    for (i = 0; i < room->numDoors; i++){
        room->doors[i]->e.draw((Entity *)room->doors[i], shiftX, shiftY);
    }
}

static void drawRoomEntities(Room *room, double shiftX, double shiftY){
    size_t i;
    for (i = 0; i < room->numEntities; i++){
        if (room->entities[i]->active){
            room->entities[i]->draw(room->entities[i], shiftX, shiftY);
        }
    }
}

static void drawRoomEnemies(Room *room, double shiftX, double shiftY){
    int i;
    for (i = 0; i < room->numEnemies; i++){
        if (room->enemies[i]->e.active){
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

Entity **getRoomEntityList(){
    return _current_area.currentRoom->entities;
}

int getNumRoomEnemies(){
    return _current_area.currentRoom->numEnemies;
}

Enemy **getRoomEnemyList(){
    return _current_area.currentRoom->enemies;
}

size_t getNumRoomDoors(){
    return _current_area.currentRoom->numDoors;
}

Door **getRoomDoorList(){
    return _current_area.currentRoom->doors;
}


/////////////////////////////////////////////////
// Demo
/////////////////////////////////////////////////
static Room *createFirstDemoRoom(){
    Room *firstRoom = init_Room(malloc(sizeof(Room)));
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
    // firstRoom->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    drawRoomBuffers(firstRoom);
    generateWallList(firstRoom, _current_area.tilesheet.tileWidth);
    
    firstRoom->numEntities = 0;
    firstRoom->numEnemies = 1;
//    firstRoom->enemies = malloc(sizeof(Enemy) * firstRoom->numEnemies);
    firstRoom->enemyIds = malloc(sizeof(size_t) * firstRoom->numEnemies);
    firstRoom->enemyInitialX = malloc(sizeof(double) * firstRoom->numEnemies);
    firstRoom->enemyInitialY = malloc(sizeof(double) * firstRoom->numEnemies);

//    firstRoom->enemies[0] = createOctorok(_current_area.sprites[0]);
//    firstRoom->enemies[0] = createOctorok();
    firstRoom->enemyIds[0] = 0;
    firstRoom->enemyInitialX[0] = 1;
    firstRoom->enemyInitialY[0] = 1;
    
    return firstRoom;
}

static Room *createSecondDemoRoom(){
    Room *room = init_Room(malloc(sizeof(Room)));
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
    // room->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    drawRoomBuffers(room);
    generateWallList(room, _current_area.tilesheet.tileWidth);

    room->numEntities = 0;
    room->numEnemies = 0;

    return room;
}

static Room *createThirdDemoRoom(){
    //61 & 63
    Room *room = init_Room(malloc(sizeof(Room)));
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
    room->tileIndices[(ROOM_WIDTH/2) + ROOM_WIDTH * (ROOM_HEIGHT-1)] = 20;
    clearFlag(room, (ROOM_WIDTH/2) + ROOM_WIDTH * (ROOM_HEIGHT-1), ROOMF_IMPASSABLE);

    room->connectingRooms[0] = 2;
    room->connectingRooms[1] = 2;
    room->connectingRooms[2] = 2;
    room->connectingRooms[3] = 2;
    // room->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * ROOM_WIDTH, _current_area.tilesheet.tileHeight * ROOM_HEIGHT);
    generateWallList(room, _current_area.tilesheet.tileWidth);
    drawRoomBuffers(room);
    
    room->numEntities = 1;
    room->numEnemies = 0;
    
    room->entityIds = malloc(sizeof(size_t) * room->numEntities);
    room->entityInitialX = malloc(sizeof(double) * room->numEntities);
    room->entityInitialY = malloc(sizeof(double) * room->numEntities);
    room->entityIds[0] = 0;
    room->entityInitialX[0] = 30;
    room->entityInitialY[0] = 30;
    
    //PIZZA - graphics for doors a little buggered atm
    // Sprite *doorSprite = loadAnimatedSprite("gfx/door1.png", TILE_SIZE);
    room->numDoors = 0;
    // room->doors = malloc(sizeof(Door *) * room->numDoors);
    // room->doors[0] = createDoor(doorSprite, UP, (ROOM_WIDTH/2) * TILE_SIZE, 0);
    // room->doors[1] = createDoor(doorSprite, DOWN, (ROOM_WIDTH/2) * TILE_SIZE, (ROOM_HEIGHT-1) * TILE_SIZE);
    
    return room;
}
