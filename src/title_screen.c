#include "title_screen.h"
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "entity.h"
#include "file_reader.h"
#include "data_reader.h"
#include "sound.h"
#include "logging.h"
#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_BUTTONS_TITLE 10

static Entity *titleName;//this is the name of the game that is projected - really just need it for the sprite/positioning
static size_t numButtons = 1;
static size_t currButton = 0;
static Entity buttons[NUM_BUTTONS_TITLE]; //PIZZA - hardcoded cause I don't see a way not to have the logic for the buttons hardcoded

static Music *backgroundMusic;
static Sound *buttonSound; //maybe this should be in the button entity?

static int seenTitleOnce = 0; //If the flag to skip the title screen at load is on, you need this so that you can return to the title screen

static int loadTitleScreenData();

void initTitleScreen(){
    if (!loadTitleScreenData()){
        LOG_ERR("Failed to load title data");
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
    if (checkAndConsumeInput(UP_BUTTON)){
        //set the current button's animation to the "up" state
        setAnimationLoop(buttons[currButton].animation, 0, 0);
        
        //change the current button
        currButton = (currButton - 1) % numButtons;
        
        //set the current button's animation to the "down" state
        setAnimationLoop(buttons[currButton].animation, 1, 0);
        
        //play a noise
        playSound(buttonSound);
    } else if (checkAndConsumeInput(DOWN_BUTTON)){
        //set the current button's animation to the "up" state
        setAnimationLoop(buttons[currButton].animation, 0, 0);
        
        //change the current button
        currButton = (currButton + 1) % numButtons;
        
        //set the current button's animation to the "down" state
        setAnimationLoop(buttons[currButton].animation, 1, 0);
        
        //play a noise
        playSound(buttonSound);
    }
    
    //play the music
    if (!musicIsPlaying()){
        playMusic(backgroundMusic);
    }
    
    //PIZZA - hardcoded to start a new game, since that's the only button right now
    //PIZZA - do we need to play some sweet animation here?
    if (checkInput(START_BUTTON) || (DEBUG_START_GAME && !seenTitleOnce)){
        seenTitleOnce = 1;
        result = 1;
        consumeAllInput();
        stopMusic();
    } else if (checkInput(ESCAPE_BUTTON)){
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
    char filename[FILENAME_BUFFER_SIZE];
    int id;
    size_t i, arrLen;
    cJSON *root, *buttonsList, *temp;
    
    //initialize stuff
    titleName = init_Entity(malloc(sizeof(Entity)));
    for (i = 0; i < numButtons; i++){
        init_Entity(buttons + i);
    }
    
    //read in the data file for the title
    char *fileContents = readFileToString(titleFilename);
    root = cJSON_Parse(fileContents);
    
    //check that the file was read correctly, then parse the data
    if (!root){
        LOG_ERR("Error reading title data: %s", cJSON_GetErrorPtr());
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
            LOG_ERR("Error in title.data: button quantities don't match");
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

        //read the sounds/music
        id = cJSON_GetObjectItem(root, "background music")->valueint;
        sprintf(filename, "data/music/%05d.music", id);
        backgroundMusic = readMusicFromFile(filename, backgroundMusic);
        id = cJSON_GetObjectItem(root, "button sound")->valueint;
        sprintf(filename, "data/sound/%05d.sound", id);
        buttonSound = readSoundFromFile(filename, buttonSound);
        
        //free the cjson object
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    LOG_INF("Title data file read");
    return result;
}
