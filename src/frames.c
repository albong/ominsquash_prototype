#include <stdlib.h>
#include "frames.h"
#include "input.h"
#include "player.h"
#include "collisions.h"
#include "area.h"
#include "weapon.h"
#include "constants.h"
#include "../debug/hitbox_drawer.h"
#include "menu.h"

//Variables
static Frame *gameFrame;
static Frame *menuFrame;

//methods
static int gameFrameLogic(unsigned delta);
static void gameFrameDraw();

static int menuFrameLogic(unsigned delta);
static void menuFrameDraw();

void initFrames(){
    gameFrame = malloc(sizeof(Frame));
    gameFrame->logic = &gameFrameLogic;
    gameFrame->draw = &gameFrameDraw;
    
    menuFrame = malloc(sizeof(Frame));
    menuFrame->logic = &menuFrameLogic;
    menuFrame->draw = &menuFrameDraw;
    
    _currentFrame = gameFrame;
}

int gameFrameLogic(unsigned delta){
    if (_input.start && !_inputRead.start){
        _currentFrame = menuFrame;
        setInputAllRead();
        return 1;
    }
    
    //do logic, calculate next positions
    doPlayer(delta);
    doRoom(delta);
    
    //do any collisions
    /*
        Do we need to skip if changing rooms?
    */
    doCollisions();

    //update positions
    movePlayer();
    moveRoomEntities();
    moveRoomEnemies();
    
    //interact button only gets one press here
    if (_input.x){
        _inputRead.x = 1;
    }
    
    return 0;
}

void gameFrameDraw(){
    drawCurrentRoom();
    drawPlayer();
    drawHitBoxes(DRAW_MOVE_HITBOX, DRAW_INTERACT_HITBOX);
}

int menuFrameLogic(unsigned delta){
    if (_input.start && !_inputRead.start){
        setInputAllRead();
        return -1;
    }
        
    return 0;
}

void menuFrameDraw(){
    drawMenu();
}