#include "menu.h"
#include "graphics.h"
#include "constants.h"
#include "player.h"
#include "input.h"
#include "weapon.h"
#include "file_reader.h"
#include "data_reader.h"
#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

//data variables
static Entity *menuEntity;
static Entity *iconSelectorEntity;
static int numIconsPerRow, numIconsPerColumn;
static int weaponIconOffsetX, weaponIconOffsetY;
static int iconBufferSize, iconSize;
static int **iconDisplay;
static int menuOffsetX;
static int menuOffsetY;

//display variables
static int resetMenu = 1;
static int inWeaponMenu = 1; //replace with an enum for which section of the menu we're currently in
static int currX = 0;
static int currY = 0;
static int startPositionInWeaponList = 0; //increment this by numIconsPerRow if there are more weapons in the list than can be displayed

static int loadMenuData();
static int updateSelectionFromInput(); //returns 0 if nothing changed, 1 otherwise

void initMenu(){
    size_t i, j;
    
    //allocate the entities
    menuEntity = init_Entity(malloc(sizeof(Entity)));
    iconSelectorEntity = init_Entity(malloc(sizeof(Entity)));
    
    //read in the data from file, load animations
    if (!loadMenuData()){
        printf("Failed to load menu data\n");
        fflush(stdout);
    }
    
    //perform additional allocations
    iconDisplay = malloc(sizeof(int *) * numIconsPerRow);
    for (i = 0; i < numIconsPerRow; i++){
        iconDisplay[i] = malloc(sizeof(int) * numIconsPerColumn);
        for (j = 0; j < numIconsPerColumn; j++){
            iconDisplay[i][j] = -1;
        }
    }
    
    //set menu position
    menuOffsetX = (SCREEN_WIDTH / 2) - (menuEntity->sprite->frameWidth / 2);
    menuOffsetY = (SCREEN_HEIGHT / 2) - (menuEntity->sprite->frameHeight / 2);
}

void termMenu(){
    // free_Image(menuImage);
    free_Entity(menuEntity);
}

int doMenu(unsigned delta){
    size_t i;
    
    //if this is the first time through the loop since opening the menu, set the positions
    if (resetMenu){
        //set where we are in the menu and what the selector is currently on
        inWeaponMenu = 1;
        startPositionInWeaponList = 0;
        currX = 0;
        currY = 0;
        
        //setup the selector
        iconSelectorEntity->active = 1;
        
        //don't do this again until we next open the menu
        resetMenu = 0;
    }
    
    //update the animations
    menuEntity->action(menuEntity, delta);
    iconSelectorEntity->action(iconSelectorEntity, delta);
    
    //handle input - you can't hold the buttons down, but that's just a logic thing I don't feel like managing right now
    if (updateSelectionFromInput()){
        setAnimationLoop(iconSelectorEntity->animation, 0, 1);
    }
    
    //move the selector
    if (inWeaponMenu){
        iconSelectorEntity->x = weaponIconOffsetX + (currX * (iconSize + 2*iconBufferSize));
        iconSelectorEntity->y = weaponIconOffsetY + (currY * (iconSize + 2*iconBufferSize));
    }
    
    //update the weapon icon displays
    for (i = 0; i < _player_weapons.num; i++){
        //if we're able to be displayed, display
        if (i >= startPositionInWeaponList && i < (startPositionInWeaponList + numIconsPerRow*numIconsPerColumn)){
            _player_weapons.weapons[i]->icon->active = 1;
            _player_weapons.weapons[i]->icon->x = ((i-startPositionInWeaponList) % numIconsPerRow)*(iconSize + 2*iconBufferSize) + weaponIconOffsetX + iconBufferSize;
            _player_weapons.weapons[i]->icon->y = ((i-startPositionInWeaponList) / numIconsPerRow)*(iconSize + 2*iconBufferSize) + weaponIconOffsetY + iconBufferSize;
            
            //if the selector is overtop of us, play animation, otherwise reset animation and don't pass delta
            if (currX + (currY * numIconsPerRow) == i - startPositionInWeaponList){
                _player_weapons.weapons[i]->icon->action(_player_weapons.weapons[i]->icon, delta);
            } else {
                setAnimationLoop(_player_weapons.weapons[i]->icon->animation, 0, 1);
            }
        } else {
            _player_weapons.weapons[i]->icon->active = 0;
        }
    }
    
    //if you've hit start, exit the menu
    if (checkInput(START_BUTTON)){
        resetMenu = 1; //next time we do logic, reset everything
        consumeAllInput();
        return -1;
    } else {
        return 0;
    }
}

void drawMenu(){
    size_t i;
    
    //first draw the menu
    menuEntity->draw(menuEntity, menuOffsetX, menuOffsetY);
    
    //draw the weapon icons
    for (i = 0; i < _player_weapons.num; i++){
        _player_weapons.weapons[i]->icon->draw(_player_weapons.weapons[i]->icon, menuOffsetX, menuOffsetY);
    }
    
    //finally, draw the selector
    if (inWeaponMenu){
        iconSelectorEntity->draw(iconSelectorEntity, menuOffsetX, menuOffsetY);
    }
}

int loadMenuData(){
    int result = 1;
    char *menuFilename = "data/menu.data";
    cJSON *root;
    int id;
    
    //read in the menu data file
    char *fileContents = readFileToCharStar(menuFilename);
    root = cJSON_Parse(fileContents);
    
    //check that the read succeeded
    if (!root){
        printf("%s: Error before: %s\n", menuFilename, cJSON_GetErrorPtr());
        fflush(stdout);
        result = 0;
    } else {
        //load the menu animation
        id = cJSON_GetObjectItem(root, "menu animation")->valueint;
        readAnimationIntoEntity(menuEntity, id);
        
        //load the selector animations
        id = cJSON_GetObjectItem(root, "icon selector animation")->valueint;
        readAnimationIntoEntity(iconSelectorEntity, id);
        
        //get the icon display sizes
        numIconsPerRow = cJSON_GetObjectItem(root, "icons per row")->valueint;
        numIconsPerColumn = cJSON_GetObjectItem(root, "icons per column")->valueint;
        
        //get the offset into the menu sprite where the icons will be drawn
        weaponIconOffsetX = cJSON_GetObjectItem(root, "weapon icon area x offset")->valueint;
        weaponIconOffsetY = cJSON_GetObjectItem(root, "weapon icon area y offset")->valueint;
        
        //get the sizing of the icons so we can accurately determine positions
        iconBufferSize = cJSON_GetObjectItem(root, "icon buffer size")->valueint;
        iconSize = cJSON_GetObjectItem(root, "icon size")->valueint;
    }
    
    return result;
}

//take input and move about the menu accordingly
int updateSelectionFromInput(){
    int result = 0;
    
    //currently there is no scrolling, so do nothing if go too far
    if (checkAndConsumeInput(UP_BUTTON)){
        currY--;
        if (currY < 0){
            currY = 0;
        }
        result = 1;
    }
    if (checkAndConsumeInput(DOWN_BUTTON)){
        currY++;
        if (currY >= numIconsPerColumn){
            currY = numIconsPerColumn - 1;
        }
        result = 1;
    }
    
    //currently there is only one menu, so do nothing if go too far
    if (checkAndConsumeInput(LEFT_BUTTON)){
        currX--;
        if (currX < 0){
            currX = 0;
        }
        result = 1;
    }
    if (checkAndConsumeInput(RIGHT_BUTTON)){
        currX++;
        if (currX >= numIconsPerRow){
            currX = numIconsPerRow - 1;
        }
        result = 1;
    }
    
    return result;
}





/*


Maybe I should have icons representing the a/b buttons (or is it x/y?) to overlay
on the player's selected weapons.  Still not sure of how to do the ui.


*/
