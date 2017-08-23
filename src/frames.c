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
#include "title_screen.h"
#include "load_screen.h"

#include <stdlib.h>
#include <stdio.h>

//Variables
static Frame *gameFrame;
static Frame *menuFrame;
static Frame *textboxFrame;
static Frame *titleScreenFrame;
static Frame *loadScreenFrame;

//methods
static int gameFrameLogic(unsigned delta);
static void gameFrameDraw();
static int menuFrameLogic(unsigned delta);
static void menuFrameDraw();
static int textboxFrameLogic(unsigned delta);
static void textboxFrameDraw();
static int titleScreenFrameLogic(unsigned delta);
static void titleScreenFrameDraw();
static int loadScreenFrameLogic(unsigned delta);
static void loadScreenFrameDraw();

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
    
    titleScreenFrame = malloc(sizeof(Frame));
    titleScreenFrame->logic = &titleScreenFrameLogic;
    titleScreenFrame->draw = &titleScreenFrameDraw;
    titleScreenFrame->drawIfNotTop = 0;
    
    loadScreenFrame = malloc(sizeof(Frame));
    loadScreenFrame->logic = loadScreenFrameLogic;
    loadScreenFrame->draw = loadScreenFrameDraw;
    loadScreenFrame->drawIfNotTop = 0;
    
    _currentFrame = titleScreenFrame;
}

void termFrames(){
    free(gameFrame);
    free(menuFrame);
    free(textboxFrame);
    free(titleScreenFrame);
    free(loadScreenFrame);
}

int gameFrameLogic(unsigned delta){
    int newAreaId;
    
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
    } else if (_input.escape){
        setAreaIdToLoad(-1);
        setInputAllRead();
        return -1;
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
    
    newAreaId = checkChangeArea();
    if (newAreaId != -1){
        printf("game frame\n");
        fflush(stdout);
        setAreaIdToLoad(newAreaId);
        setInputAllRead();
        return -1;
    } else {
        return 0;
    }
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

int titleScreenFrameLogic(unsigned delta){
    int status = doTitleScreen(delta);
    
    if (status == 1){
        setInputAllRead();
        // _currentFrame = gameFrame;
        _currentFrame = loadScreenFrame;
        setAreaIdToLoad(0);
        return 1;
    } else if (status == -1){
        setInputAllRead();
        return -1; // will cause game to exit, better to have the exit stuff in main.c I think
    } else {
        return 0;
    }
}

void titleScreenFrameDraw(){
    drawTitleScreen();
}

int loadScreenFrameLogic(unsigned delta){
    int status = doLoadScreen(delta);
    printf("load screen\n");
    if (status == 1){
        setInputAllRead();
        _currentFrame = gameFrame;
        return 1;
    } else if (status == -1){
        setInputAllRead();
        return -1;
    } else {
        return 0;
    }
}

void loadScreenFrameDraw(){
    drawLoadScreen();
}