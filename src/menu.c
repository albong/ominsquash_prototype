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

//used to track which section of the menu we're in
typedef enum {
    WEAPON_MENU, BUTTON_MENU
} MenuSection;

//data variables
static Entity *menuEntity;
static Entity *iconSelectorEntity;
static Entity *buttonSelectorEntity;
static Entity *saveButtonEntity;
static Entity *loadButtonEntity;
static Entity *exitButtonEntity;
static int numIconsPerRow, numIconsPerColumn;
static int weaponIconOffsetX, weaponIconOffsetY;
static int iconBufferSize, iconSize;
static int **iconDisplay;
static int buttonOffsetX, buttonOffsetY;
static int buttonBufferSize, buttonHeight;
static int menuOffsetX, menuOffsetY;

//display variables
static int resetMenu = 1;
static MenuSection currentSection;
static int currX = 0;
static int currY = 0;
static int startPositionInWeaponList = 0; //increment this by numIconsPerRow if there are more weapons in the list than can be displayed

//methods
static int loadMenuData();
static int updateSelectionFromInput(); //returns 0 if nothing changed, 1 otherwise

void initMenu(){
    size_t i, j;
    
    //allocate the entities
    menuEntity = init_Entity(malloc(sizeof(Entity)));
    iconSelectorEntity = init_Entity(malloc(sizeof(Entity)));
    buttonSelectorEntity = init_Entity(malloc(sizeof(Entity)));
    saveButtonEntity = init_Entity(malloc(sizeof(Entity)));
    loadButtonEntity = init_Entity(malloc(sizeof(Entity)));
    exitButtonEntity = init_Entity(malloc(sizeof(Entity)));
    
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
    
    //set the button positions
    saveButtonEntity->x = buttonBufferSize + buttonOffsetX;
    saveButtonEntity->y = buttonBufferSize + buttonOffsetY;
    loadButtonEntity->x = buttonBufferSize + buttonOffsetX;
    loadButtonEntity->y = buttonBufferSize + buttonOffsetY + (buttonHeight + 2*buttonBufferSize);
    exitButtonEntity->x = buttonBufferSize + buttonOffsetX;
    exitButtonEntity->y = buttonBufferSize + buttonOffsetY + 2*(buttonHeight + 2*buttonBufferSize);
}

void termMenu(){
    // free_Image(menuImage);
    free_Entity(menuEntity);
}

int doMenu(unsigned delta){
    int result;
    size_t i;
    
    //if this is the first time through the loop since opening the menu, set the positions
    if (resetMenu){
        //set where we are in the menu and what the selector is currently on
        currentSection = WEAPON_MENU;
        startPositionInWeaponList = 0;
        currX = 0;
        currY = 0;
        
        //don't do this again until we next open the menu
        resetMenu = 0;
    }
    
    //update the animations, even if not displayed
    menuEntity->action(menuEntity, delta);
    iconSelectorEntity->action(iconSelectorEntity, delta);
    buttonSelectorEntity->action(buttonSelectorEntity, delta);
    
    //handle input - you can't hold the buttons down, but that's just a logic thing I don't feel like managing right now
    //reset all animations regardless of whether or not its drawn - why not
    if (updateSelectionFromInput()){
        setAnimationLoop(iconSelectorEntity->animation, 0, 1);
        setAnimationLoop(buttonSelectorEntity->animation, 0, 1);
    }
    
    //move the selector
    if (currentSection == WEAPON_MENU){
        iconSelectorEntity->x = weaponIconOffsetX + (currX * (iconSize + 2*iconBufferSize));
        iconSelectorEntity->y = weaponIconOffsetY + (currY * (iconSize + 2*iconBufferSize));
    } else if (currentSection == BUTTON_MENU){
        buttonSelectorEntity->x = buttonOffsetX;
        buttonSelectorEntity->y = buttonOffsetY + (currY * (buttonHeight + 2*buttonBufferSize));
    }
    
    //update the weapon icon displays
    for (i = 0; i < _player_weapons.num; i++){
        //if we're able to be displayed, display
        if (i >= startPositionInWeaponList && i < (startPositionInWeaponList + numIconsPerRow*numIconsPerColumn)){
            _player_weapons.weapons[i]->icon->active = 1;
            _player_weapons.weapons[i]->icon->x = ((i-startPositionInWeaponList) % numIconsPerRow)*(iconSize + 2*iconBufferSize) + weaponIconOffsetX + iconBufferSize;
            _player_weapons.weapons[i]->icon->y = ((i-startPositionInWeaponList) / numIconsPerRow)*(iconSize + 2*iconBufferSize) + weaponIconOffsetY + iconBufferSize;
            
            //if the selector is overtop of us, play animation, otherwise reset animation and don't pass delta
            if (currentSection == WEAPON_MENU){
                if (currX + (currY * numIconsPerRow) == i - startPositionInWeaponList){
                    _player_weapons.weapons[i]->icon->action(_player_weapons.weapons[i]->icon, delta);
                } else {
                    setAnimationLoop(_player_weapons.weapons[i]->icon->animation, 0, 1);
                }
            }
        } else {
            _player_weapons.weapons[i]->icon->active = 0;
        }
    }
    
    //if you've hit start, exit the menu
    if (checkInput(START_BUTTON)){
        resetMenu = 1; //next time we do logic, reset everything
        consumeAllInput();
        result = -1;
    } else if (currentSection == BUTTON_MENU && checkAndConsumeInput(X_BUTTON)){
        if (currY == 2){ //exit button
            resetMenu = 1; //next time we do logic, reset everything
            result = -2;
        }
    } else {
        result = 0;
    }
    
    return result;
}

void drawMenu(){
    size_t i;
    
    //first draw the menu
    menuEntity->draw(menuEntity, menuOffsetX, menuOffsetY);
    
    //draw the weapon icons
    for (i = 0; i < _player_weapons.num; i++){
        _player_weapons.weapons[i]->icon->draw(_player_weapons.weapons[i]->icon, menuOffsetX, menuOffsetY);
    }
    
    //draw the buttons
    saveButtonEntity->draw(saveButtonEntity, menuOffsetX, menuOffsetY);
    loadButtonEntity->draw(loadButtonEntity, menuOffsetX, menuOffsetY);
    exitButtonEntity->draw(exitButtonEntity, menuOffsetX, menuOffsetY);
    
    //finally, draw the selector
    if (currentSection == WEAPON_MENU){
        iconSelectorEntity->draw(iconSelectorEntity, menuOffsetX, menuOffsetY);
    } else if (currentSection == BUTTON_MENU){
        buttonSelectorEntity->draw(buttonSelectorEntity, menuOffsetX, menuOffsetY);
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
        id = cJSON_GetObjectItem(root, "button selector animation")->valueint;
        readAnimationIntoEntity(buttonSelectorEntity, id);
        
        //load the button animations
        id = cJSON_GetObjectItem(root, "save button animation")->valueint;
        readAnimationIntoEntity(saveButtonEntity, id);
        id = cJSON_GetObjectItem(root, "load button animation")->valueint;
        readAnimationIntoEntity(loadButtonEntity, id);
        id = cJSON_GetObjectItem(root, "exit button animation")->valueint;
        readAnimationIntoEntity(exitButtonEntity, id);
        
        //get the icon display sizes
        numIconsPerRow = cJSON_GetObjectItem(root, "icons per row")->valueint;
        numIconsPerColumn = cJSON_GetObjectItem(root, "icons per column")->valueint;
        
        //get the offset into the menu sprite where the icons will be drawn
        weaponIconOffsetX = cJSON_GetObjectItem(root, "weapon icon area x offset")->valueint;
        weaponIconOffsetY = cJSON_GetObjectItem(root, "weapon icon area y offset")->valueint;
        
        //get the sizing of the icons so we can accurately determine positions
        iconBufferSize = cJSON_GetObjectItem(root, "icon buffer size")->valueint;
        iconSize = cJSON_GetObjectItem(root, "icon size")->valueint;
        
        //get the offset into the menu sprite where buttons will be drawn
        buttonOffsetX = cJSON_GetObjectItem(root, "button area x offset")->valueint;
        buttonOffsetY = cJSON_GetObjectItem(root, "button area y offset")->valueint;
        
        //get the sizing of the buttons so we can accurately determine positions
        buttonBufferSize = cJSON_GetObjectItem(root, "button buffer size")->valueint;
        buttonHeight = cJSON_GetObjectItem(root, "button height")->valueint;
    }
    
    return result;
}

//take input and move about the menu accordingly
int updateSelectionFromInput(){
    int result = 0;
    
    //
    // PIZZA - ultimately we should use percentages of screen position to decide where to go when shifting sections
    //
    
    //first, just check the input
    if (checkAndConsumeInput(UP_BUTTON)){
        currY--;
        result = 1;
    } else if (checkAndConsumeInput(DOWN_BUTTON)){
        currY++;
        result = 1;
    } else if (checkAndConsumeInput(LEFT_BUTTON)){
        currX--;
        result = 1;
    } else if (checkAndConsumeInput(RIGHT_BUTTON)){
        currX++;
        result = 1;
    }
    
    //adjust our current X and Y in whatever section (or change section) accordingly
    if (currentSection == WEAPON_MENU){
        //there is nothing for scrolling yet, so just don't go too far up or down
        if (currY < 0){
            currY = 0;
        } else if (currY >= numIconsPerColumn){
            currY = numIconsPerColumn - 1;
        }
        
        //if you scroll too far left or right, change sections
        if (currX < 0 || currX >= numIconsPerRow){
            currX = 0;
            currY = 0;
            currentSection = BUTTON_MENU;
        }
    } else if (currentSection == BUTTON_MENU){
        //there is no item menu yet, so just don't go too far up or down
        if (currY < 0){
            currY = 0;
        } else if (currY >= 3){ //only 3 buttons I guess
            currY--;
        }
        
        //there's only one column of buttons
        if (currX < 0){ //moved left
            currX = numIconsPerRow - 1;
            currY = 0;
            currentSection = WEAPON_MENU;
        } else if (currX > 0){ //moved right
            currX = 0;
            currY = 0;
            currentSection = WEAPON_MENU;
        }
    }
    
    return result;
}





/*


Maybe I should have icons representing the a/b buttons (or is it x/y?) to overlay
on the player's selected weapons.  Still not sure of how to do the ui.


*/
