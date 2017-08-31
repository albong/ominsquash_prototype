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
#include "screen_wipe.h"

#include <stdlib.h>
#include <stdio.h>

//Variables
static Frame *gameFrame;
static Frame *menuFrame;
static Frame *textboxFrame;
static Frame *titleScreenFrame;
static Frame *loadScreenFrame;
static Frame *screenWipeFrame;

//methods
static Frame *allocateFrame(int (* logic)(unsigned delta), void (* draw)(), int drawIfNotTop);
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
static int screenWipeFrameLogic(unsigned delta);
static void screenWipeFrameDraw();

void initFrames(){
    gameFrame = allocateFrame(&gameFrameLogic, &gameFrameDraw, 1);
    menuFrame = allocateFrame(&menuFrameLogic, &menuFrameDraw, 1);
    textboxFrame = allocateFrame(&textboxFrameLogic, &textboxFrameDraw, 1);
    titleScreenFrame = allocateFrame(&titleScreenFrameLogic, &titleScreenFrameDraw, 0);
    loadScreenFrame = allocateFrame(&loadScreenFrameLogic, &loadScreenFrameDraw, 0);
    screenWipeFrame = allocateFrame(&screenWipeFrameLogic, &screenWipeFrameDraw, 0);
    
    _currentFrame = titleScreenFrame;
}

Frame *allocateFrame(int (* logic)(unsigned delta), void (* draw)(), int drawIfNotTop){
    Frame *result = malloc(sizeof(Frame));
    result->logic = logic;
    result->draw = draw;
    result->drawIfNotTop = drawIfNotTop;
}

void termFrames(){
    free(gameFrame);
    free(menuFrame);
    free(textboxFrame);
    free(titleScreenFrame);
    free(loadScreenFrame);
    free(screenWipeFrame);
}

int gameFrameLogic(unsigned delta){
    int newAreaId, pushWipe;
    double x, y;
    
    if (checkInput(START_BUTTON)){
        _currentFrame = menuFrame;
        consumeAllInput();
        return 1;
    } else if (checkInput(Y_BUTTON)){
        addTextToTextbox("If the measure of a space is finite, then convergence\n in measure implies convergence\n almost anywhere implies...");
        // testTextbox();
        _currentFrame = textboxFrame;
        consumeAllInput();
        return 1;
    } else if (checkInput(ESCAPE_BUTTON)){
        setAreaIdToLoad(-1);
        consumeAllInput();
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
    checkAndConsumeInput(X_BUTTON);

    //check if we need to push on a wipe or change areas
    newAreaId = checkChangeArea();
    pushWipe = checkScreenWipe(&x, &y);
    
    //we should switch areas over performing a wipe, doCurrentRoom should accordingling manage variables to make this work correctly
    if (newAreaId != -1){ //change area
        setAreaIdToLoad(newAreaId);
        consumeAllInput();
        return -1;
    } else if (pushWipe == 1){ //wipe inward
        setScreenWipeCenter(x, y);
        setScreenWipeInward(1);
        _currentFrame = screenWipeFrame;
        return 1;
    } else if (pushWipe == 2){ //wipe outward
        setScreenWipeCenter(x, y);
        setScreenWipeInward(0);
        _currentFrame = screenWipeFrame;
        return 1;
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
    if (doMenu(delta) == -1){
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
        consumeAllInput();
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
        consumeAllInput();
        // _currentFrame = gameFrame;
        _currentFrame = loadScreenFrame;
        setAreaIdToLoad(0);
        return 1;
    } else if (status == -1){
        consumeAllInput();
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
    
    if (status == 1){
        consumeAllInput();
        _currentFrame = gameFrame;
        return 1;
    } else if (status == -1){
        consumeAllInput();
        return -1;
    } else {
        return 0;
    }
}

void loadScreenFrameDraw(){
    drawLoadScreen();
}

int screenWipeFrameLogic(unsigned delta){
    if (doScreenWipe(delta)){
        consumeAllInput();
        return -1;
    } else {
        return 0;
    }
}

void screenWipeFrameDraw(){
    drawScreenWipe();
}


