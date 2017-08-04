#include "title.h"
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "entity.h"
#include "file_reader.h"
#include "data_reader.h"
#include "../lib/cJSON/cJSON.h"
#include <stdio.h>

static Entity *titleName;//this is the name of the game that is projected - really just need it for the sprite/positioning

static int loadTitleData();

void initTitle(){
    if (!loadTitleData()){
        //an error I guess
    }
}

int doTitle(unsigned delta){
    titleName->action(titleName, delta);
    
    return 0;
}

void drawTitle(){
    drawFilledRect_T(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 10);
    titleName->draw(titleName, 0, 0);
}

int loadTitleData(){
    int result = 1;
    char *titleFilename = "data/title.data";
    char filename[80];
    int id;
    
    //initialize the name of the game
    titleName = init_Entity(malloc(sizeof(Entity)));
    
    //read in the data file for the title
    char *fileContents = readFileToCharStar(titleFilename);
    cJSON *root = cJSON_Parse(fileContents);
    
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
        
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    
    return result;
}