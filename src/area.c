#include <stdlib.h> 

#include "area.h"
#include "room.h"
#include "graphics.h"

#include "SDL/SDL.h"

void loadArea(){
    //load the tilesheet
    _current_area.tilesetName = "gfx/area1tiles.png";
    _current_area.tilesheet.sheet = loadImage(_current_area.tilesetName);
    _current_area.tilesheet.tileWidth = 16;
    _current_area.tilesheet.tileHeight = 16;
        
    //load the rooms
    _current_area.numRooms = 1;
    _current_area.roomList = (Room**) malloc(sizeof(Room *) * _current_area.numRooms);
    
    Room *firstRoom = malloc(sizeof(Room));
    firstRoom->width = 25;
    firstRoom->height = 20;
    firstRoom->tileIndices = (int *) malloc(sizeof(int) * firstRoom->height * firstRoom->width);
    int i = 0;
    for (i = 0; i < firstRoom->height * firstRoom->width; i++){
        firstRoom->tileIndices[i] = 41; 
    }
    firstRoom->buffer = getEmptySurface(_current_area.tilesheet.tileWidth * firstRoom->width, _current_area.tilesheet.tileHeight * firstRoom->height);
    drawRoomBuffers(firstRoom);
    
    _current_area.roomList[0] = firstRoom;
    _current_area.currentRoom = 0;
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
    drawImage(_current_area.roomList[_current_area.currentRoom]->buffer, 0, 0);
}

/*
draw room to buffers - one buffer per frame of animation
*/
