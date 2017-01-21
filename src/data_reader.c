#include "data_reader.h"
#include "area.h"
#include "room.h"
#include "constants.h"
#include "entity.h"
#include "hitbox.h"
#include "graphics.h"

#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <string.h>

static char *readFileToCharStar(char *filename);

//read in a JSON file to a cJSON object
char *readFileToCharStar(char *filename){
    //More or less copied and pasted from the cJSON examples
    
    // cJSON *result;
    FILE *f;
    long len;
    char *data;

    // open in read binary mode
    f = fopen(filename,"rb");
    // get the length
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = (char*)malloc(len + 1);

    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    // result = cJSON_Parse(data);
    //PIZZA - Should this be freed after parsing the json?  Dr Memory indicates yes?
    // free(data);
    return data;
}

Area *readAreaFromFile(char *filename, Area *result){
    //init the area
    if (result == NULL){
        result = malloc(sizeof(Area));
    }
    init_Area(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    cJSON *root = cJSON_Parse(fileContents);
    if (!root){
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        free(result);
        free(fileContents);
        return NULL;
    }
    
    //fill in the object
    result->tilesetName = strdup(cJSON_GetObjectItem(root, "tilesheet")->valuestring);
    // result->numRooms = cJSON_GetObjectItem(root, "number of rooms")->valueint;
    
    //allocate for room array
    cJSON *roomList = cJSON_GetObjectItem(root, "rooms");
    result->numRooms = cJSON_GetArraySize(roomList);
    result->roomList = malloc(sizeof(Room *) * result->numRooms);
    
    //init and fill in rooms
    Room *currRoom;
    cJSON *arrY, *arrX, *jsonRoom, *jsonTemp;
    size_t k;
    int i, j;
    for (k = 0; k < result->numRooms; k++){
        result->roomList[k] = init_Room(malloc(sizeof(Room)));
        currRoom = result->roomList[k];
        jsonRoom = cJSON_GetArrayItem(roomList, k);
        
        //allocate for the arrays of tiles and flags
        currRoom->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
        currRoom->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
        
        //set the connecting rooms
        currRoom->connectingRooms[0] = cJSON_GetObjectItem(jsonRoom, "up connecting room")->valueint;
        currRoom->connectingRooms[1] = cJSON_GetObjectItem(jsonRoom, "down connecting room")->valueint;
        currRoom->connectingRooms[2] = cJSON_GetObjectItem(jsonRoom, "left connecting room")->valueint;
        currRoom->connectingRooms[3] = cJSON_GetObjectItem(jsonRoom, "right connecting room")->valueint;
        
        //set the tile indices
        arrY = cJSON_GetObjectItem(jsonRoom, "tile layout");
        for (j = 0; j < cJSON_GetArraySize(arrY); j++){
            arrX = cJSON_GetArrayItem(arrY, j);
            for (i = 0; i < cJSON_GetArraySize(arrX); i++){
                currRoom->tileIndices[i + (j * ROOM_WIDTH)] = cJSON_GetArrayItem(arrX, i)->valueint;
            }
        }
        
        //set the flags for the walls
        arrY = cJSON_GetObjectItem(jsonRoom, "wall layout");
        for (j = 0; j < cJSON_GetArraySize(arrY); j++){
            arrX = cJSON_GetArrayItem(arrY, j);
            for (i = 0; i < cJSON_GetArraySize(arrX); i++){
                if (cJSON_GetArrayItem(arrX, i)->valueint){
                    setFlag(currRoom, i + (j * ROOM_WIDTH), ROOMF_IMPASSABLE);
                }
            }
        }
        generateWallList(currRoom, TILE_SIZE);

        //allocate and load the entity lists
        arrX = cJSON_GetObjectItem(jsonRoom, "entity positions");
        currRoom->numEntities = (size_t)cJSON_GetArraySize(arrX);
        currRoom->entityIds = malloc(sizeof(size_t) * currRoom->numEntities);
        currRoom->entityInitialX = malloc(sizeof(double) * currRoom->numEntities);
        currRoom->entityInitialY = malloc(sizeof(double) * currRoom->numEntities);
        for (i = 0; i < cJSON_GetArraySize(arrX); i++){
            jsonTemp = cJSON_GetArrayItem(arrX, i);
            currRoom->entityIds[i] = cJSON_GetObjectItem(jsonTemp, "ID")->valueint;
            currRoom->entityInitialX[i] = cJSON_GetObjectItem(jsonTemp, "x")->valueint;
            currRoom->entityInitialY[i] = cJSON_GetObjectItem(jsonTemp, "y")->valueint;
        }
        
        //allocate and load enemy lists
        arrX = cJSON_GetObjectItem(jsonRoom, "enemy positions");
        currRoom->numEnemies = (size_t)cJSON_GetArraySize(arrX);
        currRoom->enemyIds = malloc(sizeof(size_t) * currRoom->numEnemies);
        currRoom->enemyInitialX = malloc(sizeof(double) * currRoom->numEnemies);
        currRoom->enemyInitialY = malloc(sizeof(double) * currRoom->numEnemies);
        for (i = 0; i < cJSON_GetArraySize(arrX); i++){
            jsonTemp = cJSON_GetArrayItem(arrX, i);
            currRoom->enemyIds[i] = cJSON_GetObjectItem(jsonTemp, "ID")->valueint;
            currRoom->enemyInitialX[i] = cJSON_GetObjectItem(jsonTemp, "x")->valueint;
            currRoom->enemyInitialY[i] = cJSON_GetObjectItem(jsonTemp, "y")->valueint;
        }
    }
    result->currentRoom = result->roomList[0];
    
    //free and return
    cJSON_Delete(root);
    free(fileContents);
    return result;
}

Entity *readEntityFromFile(char *filename, Entity *result){
    //init the area
    if (result == NULL){
        result = malloc(sizeof(Entity));
    }
    init_Entity(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    cJSON *root = cJSON_Parse(fileContents);
    if (!root){
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        free(result);
        free(fileContents);
        return NULL;
    }
    
    //some entities may be disabled by default?
    if (cJSON_HasObjectItem(root, "active")){
        if (cJSON_GetObjectItem(root, "active")->type != cJSON_True){
            result->active = 0;
        }
    }
    
    //read in the various pieces of data
    result->pixelsPerMilli = cJSON_GetObjectItem(root, "speed in pixelsPerMillisecond")->valuedouble;
	result->w = cJSON_GetObjectItem(root, "width")->valueint;
    result->h = cJSON_GetObjectItem(root, "height")->valueint;
	result->milliPerFrame = cJSON_GetObjectItem(root, "milliseconds per frame")->valueint;
	result->numFrames = cJSON_GetObjectItem(root, "number of frames")->valueint;
    // result->type = <switch statement to choose correct enum based on string>;
	result->hasMoveHitBox = (cJSON_GetObjectItem(root, "has movement hitbox")->type == cJSON_True);
	result->hasInteractHitBox = (cJSON_GetObjectItem(root, "has interact hitbox")->type == cJSON_True);
    result->interactable = (cJSON_GetObjectItem(root, "interactable")->type == cJSON_True);
	
    
    //if there is a particular kind of hitbox, read it in
    cJSON *hitboxArr, *shapeArr, *hitboxJson, *shapeJson;
    size_t numHitboxes, numShapes;
    size_t i, j;
    if (result->hasMoveHitBox){
        //result->moveHitBox = NULL;
    }
    
    if (result->hasInteractHitBox){
        hitboxArr = cJSON_GetObjectItem(root, "interact hitboxes");
        numHitboxes = cJSON_GetArraySize(hitboxArr);
        result->interactHitBox = malloc(sizeof(HitBox) * numHitboxes);
        for (i = 0; i < numHitboxes; i++){
            hitboxJson = cJSON_GetArrayItem(hitboxArr, i);
            result->interactHitBox[i].numCircle = 0;
            result->interactHitBox[i].circles = NULL;
            
            shapeArr = cJSON_GetObjectItem(hitboxJson, "rectangles");
            numShapes = cJSON_GetArraySize(shapeArr);
            result->interactHitBox[i].numRect = numShapes;
            result->interactHitBox[i].rects = malloc(sizeof(CollRect) * numShapes);
            for (j = 0; j < numShapes; j++){
                shapeJson = cJSON_GetArrayItem(shapeArr, j);
                result->interactHitBox[i].rects[j].x = cJSON_GetObjectItem(shapeJson, "x")->valueint;
                result->interactHitBox[i].rects[j].y = cJSON_GetObjectItem(shapeJson, "y")->valueint;
                result->interactHitBox[i].rects[j].w = cJSON_GetObjectItem(shapeJson, "width")->valueint;
                result->interactHitBox[i].rects[j].h = cJSON_GetObjectItem(shapeJson, "height")->valueint;
            }
        }
    }
    
    //load the sprite
    //PIZZA - ultimately we will load sprite data from JSON files too, so this will be different
	result->sprite = loadAnimatedSprite(cJSON_GetObjectItem(root, "sprite")->valuestring, cJSON_GetObjectItem(root, "sprite width")->valueint);
    // result->tilesetName = strdup(cJSON_GetObjectItem(root, "tilesheet")->valuestring);
    
    //free and return
    cJSON_Delete(root);
    free(fileContents);
    return result;
}