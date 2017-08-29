#include "load_screen.h"
#include "entity.h"
#include "data_reader.h"
#include "constants.h"
#include "area.h"

#include <stdio.h>

static Entity *loadingIcon;
static int areaIdToLoad = 0;
static int hasDrawn = 0;

void initLoadScreen(){
    char dataFilename[FILENAME_BUFFER_SIZE];
    sprintf(dataFilename, "data/entities/%05d.entity", LOAD_ICON_ENTITY_ID);
    
    //PIZZA - probs just replace with an image
    loadingIcon = init_Entity(malloc(sizeof(Entity)));
    readEntityFromFile(dataFilename, loadingIcon);
    loadingIcon->x = 260;
    loadingIcon->y = 290;
}

void termLoadScreen(){
    free_Entity(loadingIcon);
}

int doLoadScreen(unsigned delta){
    int result = 0;
    
    //this is a hack to ensure that the loading icon gets drawn once - loading will block, so nothing will ever get drawn otherwise
    if (!hasDrawn){
        hasDrawn++;
    } else if (areaIdToLoad >= 0){
        //update the animation
        loadingIcon->action(loadingIcon, delta);
        
        //free previous area?
        if (checkChangeArea() != -1){
            unloadCurrentArea();
        }
        
        //load the area
        if (!loadAreaById(areaIdToLoad)){
            printf("Failed to load area %d\n", areaIdToLoad);
            result = 0;
        } else {
            result = 1;
            hasDrawn = 0;
            
            //causes the area to set variables to push an outward wipe onto the frame stack
            setWipeAfterLoadScreen();
        }
    } else {
        unloadCurrentArea();
        result = -1;
    }
    return result;
}

void drawLoadScreen(){
    //everything is single threaded, so this may never get called
    drawFilledRect_T(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0);
    loadingIcon->draw(loadingIcon, 0, 0);
}

void setAreaIdToLoad(int id){
    areaIdToLoad = id;
}