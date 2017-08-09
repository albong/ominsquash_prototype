#include "title_screen.h"
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "entity.h"
#include "file_reader.h"
#include "data_reader.h"
#include "../lib/cJSON/cJSON.h"
#include <stdio.h>

#define NUM_BUTTONS_TITLE 10

static Entity *titleName;//this is the name of the game that is projected - really just need it for the sprite/positioning
static size_t numButtons = 1;
static size_t currButton = 0;
static Entity buttons[NUM_BUTTONS_TITLE]; //PIZZA - hardcoded cause I don't see a way not to have the logic for the buttons hardcoded

static int seenTitleOnce = 0; //If the flag to skip the title screen at load is on, you need this so that you can return to the title screen

static int loadTitleScreenData();

void initTitleScreen(){
    if (!loadTitleScreenData()){
        printf("Error: failed to load title data\n");
        fflush(stdout);
    }
    
    setAnimationLoop(buttons[0].animation, 1, 0);
}

void termTitleScreen(){
    free_Entity(titleName);
    
    size_t i;
    for (i = 0; i < numButtons; i++){
        term_Entity(buttons + i);
    }
}

int doTitleScreen(unsigned delta){
    size_t i;
    int result = 0;
    
    //update the animation loops
    titleName->action(titleName, delta);
    for (i = 0; i < numButtons; i++){
        buttons[i].action(buttons + i, delta);
    }
    
    //manage input and change animation loops
    if (_input.up && !_inputRead.up){
        //set the current button's animation to the "up" state
        setAnimationLoop(buttons[currButton].animation, 0, 0);
        
        //change the current button
        currButton = (currButton - 1) % numButtons;
        _inputRead.up = 1;
        
        //set the current button's animation to the "down" state
        setAnimationLoop(buttons[currButton].animation, 1, 0);
    } else if (_input.down && !_inputRead.down){
        //set the current button's animation to the "up" state
        setAnimationLoop(buttons[currButton].animation, 0, 0);
        
        //change the current button
        currButton = (currButton + 1) % numButtons;
        _inputRead.down = 1;
        
        //set the current button's animation to the "down" state
        setAnimationLoop(buttons[currButton].animation, 1, 0);
    }
    
    //PIZZA - hardcoded to start a new game, since that's the only button right now
    //PIZZA - do we need to play some sweet animation here?
    if (_input.start || (DEBUG_START_GAME && !seenTitleOnce)){
        seenTitleOnce = 1;
        result = 1;
        setInputAllRead();
    } else if (_input.escape && !_inputRead.escape){
        result = -1;
    }
    
    return result;
}

void drawTitleScreen(){
    size_t i;
    
    //draw a blank background
    drawFilledRect_T(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 128, 128, 150);
    
    //draw the title of the game
    titleName->draw(titleName, 0, 0);
    
    //draw the buttons
    for (i = 0; i < numButtons; i++){
        buttons[i].draw(buttons + i, 0, 0);
    }
}

int loadTitleScreenData(){
    int result = 1;
    char *titleFilename = "data/title.data";
    char filename[80];
    int id;
    size_t i, arrLen;
    cJSON *root, *buttonsList, *temp;
    
    //initialize stuff
    titleName = init_Entity(malloc(sizeof(Entity)));
    for (i = 0; i < numButtons; i++){
        init_Entity(buttons + i);
    }
    
    //read in the data file for the title
    char *fileContents = readFileToCharStar(titleFilename);
    root = cJSON_Parse(fileContents);
    
    //check that the file was read correctly, then parse the data
    if (!root){
        printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
        fflush(stdout);
        result = 0;
    } else {
        //load in the entity for the name of the game
        id = cJSON_GetObjectItem(root, "name entity")->valueint;
        sprintf(filename, "data/entities/%05d.entity", id);
        titleName = readEntityFromFile(filename, titleName);
        titleName->x = cJSON_GetObjectItem(root, "name x")->valueint;
        titleName->y = cJSON_GetObjectItem(root, "name y")->valueint;
        
        //load in the buttons - if C and JSON number of buttons don't match, error
        buttonsList = cJSON_GetObjectItem(root, "buttons");
        arrLen = cJSON_GetArraySize(buttonsList);
        if (arrLen != numButtons){
            result = 0;
            printf("Error in title.data: button quantities don't match.\n");
            fflush(stdout);
        } else {
            for (i = 0; i < numButtons; i++){
                temp = cJSON_GetArrayItem(buttonsList, i);
                id = cJSON_GetObjectItem(temp, "entity")->valueint;
                sprintf(filename, "data/entities/%05d.entity", id);
                readEntityFromFile(filename, buttons + i);
                buttons->x = cJSON_GetObjectItem(temp, "x")->valueint;
                buttons->y = cJSON_GetObjectItem(temp, "y")->valueint;
            }
        }
        
        //free the csjon object
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    
    return result;
}