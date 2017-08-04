#include "frames.h"
#include "input.h"
#include "player.h"
#include "collisions.h"
#include "area.h"
#include "weapon.h"
#include "constants.h"
#include "../debug/hitbox_drawer.h"
#include "menu.h"
#include "textbox.h"
#include "title.h"

#include <stdlib.h>
#include <stdio.h>

//Variables
static Frame *gameFrame;
static Frame *menuFrame;
static Frame *textboxFrame;
static Frame *titleFrame;

//methods
static int gameFrameLogic(unsigned delta);
static void gameFrameDraw();
static int menuFrameLogic(unsigned delta);
static void menuFrameDraw();
static int textboxFrameLogic(unsigned delta);
static void textboxFrameDraw();
static int titleFrameLogic(unsigned delta);
static void titleFrameDraw();


void initFrames(){
    gameFrame = malloc(sizeof(Frame));
    gameFrame->logic = &gameFrameLogic;
    gameFrame->draw = &gameFrameDraw;
    gameFrame->drawIfNotTop = 1;
    
    menuFrame = malloc(sizeof(Frame));
    menuFrame->logic = &menuFrameLogic;
    menuFrame->draw = &menuFrameDraw;
    menuFrame->drawIfNotTop = 1;
    
    textboxFrame = malloc(sizeof(Frame));
    textboxFrame->logic = &textboxFrameLogic;
    textboxFrame->draw = &textboxFrameDraw;
    textboxFrame->drawIfNotTop = 1;
    
    titleFrame = malloc(sizeof(Frame));
    titleFrame->logic = &titleFrameLogic;
    titleFrame->draw = &titleFrameDraw;
    titleFrame->drawIfNotTop = 0;
    
    _currentFrame = titleFrame;
}

int gameFrameLogic(unsigned delta){
    if (_input.start && !_inputRead.start){
        _currentFrame = menuFrame;
        setInputAllRead();
        return 1;
    } else if (_input.y && !_inputRead.y){
        addTextToTextbox("If the measure of a space is finite, then convergence\n in measure implies convergence\n almost anywhere implies...");
        // testTextbox();
        _currentFrame = textboxFrame;
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
    moveTemporaryEntities();
    
    //update interface
    updateInterface(delta);
    
    //interact button only gets one press here
    if (_input.x){
        _inputRead.x = 1;
    }
    
    return 0;
}

void gameFrameDraw(){
    drawCurrentRoom();
    drawPlayer();
    drawCurrentRoomTopLayer();
    drawHitboxes(DRAW_MOVE_HITBOX, DRAW_INTERACT_HITBOX);
    drawInterface();
}

int menuFrameLogic(unsigned delta){
    if (_input.start && !_inputRead.start){
        setInputAllRead();
        return -1;
    } else {
        return 0;
    }
}

void menuFrameDraw(){
    drawMenu();
}

int textboxFrameLogic(unsigned delta){
    //if true, text is done being read
    if (doTextbox(delta)){
        setInputAllRead();
        return -1;
    } else {    
        return 0;
    }
}

void textboxFrameDraw(){
    drawTextbox();
}

int titleFrameLogic(unsigned delta){
    if (doTitle(delta)){
        setInputAllRead();
        _currentFrame = gameFrame;
        return 1;
    } else {
        return 0;
    }
}

void titleFrameDraw(){
    drawTitle();
}
