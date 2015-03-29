#include <stdlib.h> 

#include "constants.h"
#include "area.h"
#include "room.h"
#include "graphics.h"
#include "player.h"

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

void loadArea(){
    //load the tilesheet
    _current_area.tilesetName = "gfx/area1tiles.png";
    _current_area.tilesheet.sheet = loadImage(_current_area.tilesetName);
    _current_area.tilesheet.tileWidth = 16;
    _current_area.tilesheet.tileHeight = 16;
        
    //load the rooms
    _current_area.numRooms = 1;
    _current_area.roomList = (Room**) malloc(sizeof(Room *) * _current_area.numRooms);
    _current_area.roomList[0] = createFirstDemoRoom();
    _current_area.roomList[1] = createSecondDemoRoom();
    _current_area.currentRoom = _current_area.roomList[1];
}

static void drawRoomBuffers(Room *room){
//    int currentRoom = _current_area.currentRoom;
//    Room *room = _current_area.roomList[currentRoom];
    int roomWidth = room->width;
    int roomHeight = room->height;
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

void drawCurrentRoom(){
    if (!changingRooms){
        drawImage(_current_area.currentRoom->buffer, 0, 0);
    } else {
        drawImage(_current_area.currentRoom->buffer, room_transition.oldX, room_transition.oldY);
        drawImage(room_transition.newRoom->buffer, room_transition.newX, room_transition.newY);
    }
}

void doRoom(int delta){
    totalDelta += delta;
    int newRoom;
    if (!changingRooms){
        newRoom = checkForRoomChange();
        if (newRoom >= 0 && _current_area.currentRoom->connectingRooms[newRoom] != -1){
            totalDelta = 0;
            changingRooms = 1;
            changingToRoom = newRoom;
            changeRoom(_current_area.currentRoom->connectingRooms[changingToRoom], changingToRoom, delta);
            room_transition.newRoom = _current_area.roomList[_current_area.currentRoom->connectingRooms[newRoom]];
            setPlayerTransitioning(changingToRoom);
        }
    } else {
        changeRoom(_current_area.currentRoom->connectingRooms[changingToRoom], changingToRoom, delta);
        
    }
}

static int checkForRoomChange(){
    //eventually use player's ground bounding box for this
    double left = player.x + (player.sprite->width * 0.4);
    double right = player.x + (player.sprite->width * 0.6);
    double up = player.y + (player.sprite->image->h * 0.4);
    double down = player.y + (player.sprite->image->h * 0.6);
    
    int roomRight = _current_area.tilesheet.tileWidth * _current_area.currentRoom->width;
    int roomDown = _current_area.tilesheet.tileHeight * _current_area.currentRoom->height;

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

void changeRoom(int roomIndex, int direction, int delta){
    //calculate the x and y at which to draw the current room and the next room
    //we'll know the transition is over because we know how much we started with (screen width), and we know how many pixels we've moved
    //should put all of this into a struct
    //then we we go to draw the room, if we're transitioning, we'll draw the image of the two rooms, else just draw normal
    //probably should create a camera object to deal with the scrolling room?  may be a tutorial about this 
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

static Room *createFirstDemoRoom(){
    Room *firstRoom = malloc(sizeof(Room));
    firstRoom->width = 25;
    firstRoom->height = 20;
    firstRoom->tileIndices = (int *) malloc(sizeof(int) * firstRoom->height * firstRoom->width);
    firstRoom->flags = (uint32_t *) malloc(sizeof(uint32_t) * firstRoom->height * firstRoom->width);
    int i;
    for (i = 0; i < firstRoom->height * firstRoom->width; i++){
        firstRoom->tileIndices[i] = 41;
        firstRoom->flags[i] = 0;
    }
    firstRoom->connectingRooms[0] = 1;
    firstRoom->connectingRooms[1] = 1;
    firstRoom->connectingRooms[2] = 1;
    firstRoom->connectingRooms[3] = 1;
    firstRoom->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * firstRoom->width, _current_area.tilesheet.tileHeight * firstRoom->height);
    drawRoomBuffers(firstRoom);
    return firstRoom;
}

static Room *createSecondDemoRoom(){
    Room *room = malloc(sizeof(Room));
    room->width = 25;
    room->height = 20;
    room->tileIndices = (int *) malloc(sizeof(int) * room->height * room->width);
    room->flags = (uint32_t *) malloc(sizeof(uint32_t) * room->height * room->width);
    int i;
    for (i = 0; i < room->height * room->width; i++){
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
    room->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * room->width, _current_area.tilesheet.tileHeight * room->height);
    drawRoomBuffers(room);
    return room;
}
