#include "data_reader.h"
#include "area.h"
#include "room.h"
#include "stair.h"
#include "constants.h"
#include "entity.h"
#include "hitbox.h"
#include "graphics.h"
#include "entity_creator.h"

#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <string.h>

//PIZZA - Needs to check for existence of all fields being read!

static char *readFileToCharStar(char *filename);
//return 0 on failure
static int fillAreaFromJson(cJSON *root, Area *result);
static int fillEntityFromJson(cJSON *root, Entity *result);
static int fillEnemyFromJson(cJSON *root, Enemy *result);
static int fillSpriteFromJson(cJSON *root, Sprite *result);
static int fillAnimationFromJson(cJSON *root, Animation *result);
static int fillHitboxesFromJson(cJSON *root, Hitboxes *result);
static int addAnimationToEntity(Entity *result, int animationId);

//read in a JSON file to a cJSON object
char *readFileToCharStar(char *filename){
    //More or less copied and pasted from the cJSON examples
    
    // cJSON *result;
    FILE *f;
    long len;
    char *data;

    // open in read binary mode
    f = fopen(filename,"rb");
    if (f == NULL){
        return NULL;
    }
    
    // get the length
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    //allocate and read the data
    data = (char*)malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    
    fclose(f);
    
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
    
    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root || !fillAreaFromJson(root, result)){
        printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
        fflush(stdout);
        free(result);
        // return NULL;
        result = NULL;
    } else {
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    return result;
}

Entity *readEntityFromFile(char *filename, Entity *result){
    //init the entity
    if (result == NULL){
        result = malloc(sizeof(Entity));
    }
    init_Entity(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    cJSON *root = cJSON_Parse(fileContents);

    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root || !fillEntityFromJson(root, result)){
        printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
        fflush(stdout);
        free(result);
        result = NULL;
    } else {
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    return result;
}

Enemy *readEnemyFromFile(char *filename, Enemy *result){
    //init the entity
    if (result == NULL){
        result = malloc(sizeof(Enemy));
    }
    init_Enemy(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    cJSON *root = cJSON_Parse(fileContents);

    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root || !fillEnemyFromJson(root, result)){
        printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
        fflush(stdout);
        free(result);
        result = NULL;
    } else {
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    return result;
}

Sprite *readSpriteFromFile(char *filename, Sprite *result){
    //init the sprite
    if (result == NULL){
        result = malloc(sizeof(Sprite));
    }
    init_Sprite(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    cJSON *root = cJSON_Parse(fileContents);
    
    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root || !fillSpriteFromJson(root, result)){
        printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
        fflush(stdout);
        free(result);
        result = NULL;
    } else {
        cJSON_Delete(root);
    }
    
    //free the read file and return
    free(fileContents);
    return result;
}

Animation *readAnimationFromFile(char *filename, Animation *result){
    //init the animation
    if (result == NULL){
        result = malloc(sizeof(Animation));
    }
    init_Animation(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToCharStar(filename);
    if (fileContents == NULL){
        printf("Not found: %s\n", filename);
        fflush(stdout);
        free(result);
        result = NULL;
    } else {   
        //parse the file
        cJSON *root = cJSON_Parse(fileContents);
        
        //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
        //otherwise free the cJSON root correctly
        if (!root || !fillAnimationFromJson(root, result)){
            printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
            fflush(stdout);
            free(result);
            result = NULL;
        } else {
            cJSON_Delete(root);
        }
    }
        
    //free the read file and return
    free(fileContents);
    return result;
}

Hitboxes *readHitboxesFromFile(char *filename, Hitboxes *result, int allowAllocate){
    //the expectation here is that the hitboxes are actually part of the memory for Entities
    //as a result, you DON'T want to malloc or free the pointer passed in, as you're
    //either going to allocate something that gets lost after returning (so a leak)
    //or you're freeing a subset of something else's memory (which is bad)
    //Therefore, if you really just want to allocate hitboxes, there's an optional
    //flag allowing for that.
    
    if (result == NULL && allowAllocate){
        result = malloc(sizeof(Hitboxes));
    } else if (result == NULL && !allowAllocate){
        return NULL;
    }
    init_Hitboxes(result);
    
    //read in file, convert to cJSON and parse
    char *fileContents = readFileToCharStar(filename);
    if (fileContents == NULL){
        printf("Not found: %s\n", filename);
        fflush(stdout);
    } else {
        //parse the file
        cJSON *root = cJSON_Parse(fileContents);
    
        //if the initial parsing failed, or the detailed parsing failed, print an error
        //then then free and NULLify if that's okay, otherwise you're going to return a faulty struct
        //otherwise free the cJSON root correctly
        if (!root || !fillHitboxesFromJson(root, result)){
            printf("%s: Error before: %s\n", filename, cJSON_GetErrorPtr());
            fflush(stdout);
            if (allowAllocate){
                free(result);
                result = NULL;
            }
        } else {
            cJSON_Delete(root);
        }
    }
    
    //free the read file and return
    free(fileContents);
    return result;
}

int fillAreaFromJson(cJSON *root, Area *result){
    //fill in the object
    result->tilesetName = strdup(cJSON_GetObjectItem(root, "tilesheet")->valuestring);
    // result->numRooms = cJSON_GetObjectItem(root, "number of rooms")->valueint;
    
    //allocate for room array
    cJSON *roomList = cJSON_GetObjectItem(root, "rooms");
    result->numRooms = cJSON_GetArraySize(roomList);
    result->roomList = malloc(sizeof(Room *) * result->numRooms);
    
    //init and fill in rooms
    Room *currRoom;
    Stair *tempStair;
    cJSON *arrY, *arrX, *jsonRoom, *jsonTemp, *arrStair;
    size_t k;
    int i, j, animationId;
    for (k = 0; k < result->numRooms; k++){
        result->roomList[k] = init_Room(malloc(sizeof(Room)));
        currRoom = result->roomList[k];
        jsonRoom = cJSON_GetArrayItem(roomList, k);
        
        //allocate for the arrays of tiles and flags
        // currRoom->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
        currRoom->tileIndices = (int *) calloc(ROOM_HEIGHT * ROOM_WIDTH, sizeof(int));
        // currRoom->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
        currRoom->flags = (uint32_t *) calloc(ROOM_HEIGHT * ROOM_WIDTH, sizeof(uint32_t));
        
        //set the connecting rooms
        currRoom->connectingRooms[ROOM_UP] = cJSON_GetObjectItem(jsonRoom, "up connecting room")->valueint;
        currRoom->connectingRooms[ROOM_DOWN] = cJSON_GetObjectItem(jsonRoom, "down connecting room")->valueint;
        currRoom->connectingRooms[ROOM_LEFT] = cJSON_GetObjectItem(jsonRoom, "left connecting room")->valueint;
        currRoom->connectingRooms[ROOM_RIGHT] = cJSON_GetObjectItem(jsonRoom, "right connecting room")->valueint;
        
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
                if (cJSON_GetArrayItem(arrX, i)->valueint == ROOMF_IMPASSABLE){
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
        
        //allocate and load the stair lists
        arrStair = cJSON_GetObjectItem(jsonRoom, "stairs");
        currRoom->numStairs = (size_t)cJSON_GetArraySize(arrStair);
        currRoom->stairs = malloc(sizeof(Stair *) * currRoom->numStairs);
        for (i = 0; i < cJSON_GetArraySize(arrStair); i++){
            jsonTemp = cJSON_GetArrayItem(arrStair, i);
            tempStair = init_Stair(malloc(sizeof(Stair)));
            
            tempStair->e.active = 1;
            tempStair->e.x = cJSON_GetObjectItem(jsonTemp, "x")->valueint;
            tempStair->e.y = cJSON_GetObjectItem(jsonTemp, "y")->valueint;
            
            tempStair->sameArea = 1; //PIZZA - need to add passing of id to these methods so that you can check and correctly use this
            tempStair->toArea = cJSON_GetObjectItem(jsonTemp, "to area")->valueint;;
            tempStair->toRoom = cJSON_GetObjectItem(jsonTemp, "to room")->valueint;;
            tempStair->toTileX = cJSON_GetObjectItem(jsonTemp, "to tile x")->valueint;;
            tempStair->toTileY = cJSON_GetObjectItem(jsonTemp, "to tile y")->valueint;;
            
            animationId = cJSON_GetObjectItem(jsonTemp, "animation")->valueint;
            addAnimationToEntity((Entity *)tempStair, animationId);
            currRoom->stairs[i] = tempStair;
        }
    }
    result->currentRoom = result->roomList[0];
    
    return 1;
}

int fillEntityFromJson(cJSON *root, Entity *result){
    //some entities may be disabled by default?
    if (cJSON_HasObjectItem(root, "active")){
        if (cJSON_GetObjectItem(root, "active")->type == cJSON_True){
            result->active = 1;
        }
    }
    
    //read in the various pieces of data
    result->pixelsPerMilli = cJSON_GetObjectItem(root, "speed in pixelsPerMillisecond")->valuedouble;
	result->w = cJSON_GetObjectItem(root, "width")->valueint;
    result->h = cJSON_GetObjectItem(root, "height")->valueint;
	result->milliPerFrame = cJSON_GetObjectItem(root, "milliseconds per frame")->valueint;
	result->numFrames = cJSON_GetObjectItem(root, "number of frames")->valueint;
    // result->type = <switch statement to choose correct enum based on string>;
	// result->hasMoveHitbox = (cJSON_GetObjectItem(root, "has movement hitbox")->type == cJSON_True);
	// result->hasInteractHitbox = (cJSON_GetObjectItem(root, "has interact hitbox")->type == cJSON_True);
    result->interactable = (cJSON_GetObjectItem(root, "interactable")->type == cJSON_True);
    
    //load the sprite and animation and hitboxes
    // char dataFilename[80];
    int animationId = cJSON_GetObjectItem(root, "animation")->valueint;
    // sprintf(dataFilename, "data/sprites/%05d.sprite", animationId);
    // result->sprite = readSpriteFromFile(dataFilename, malloc(sizeof(Sprite)));
    // sprintf(dataFilename, "data/animations/%05d.animation", animationId);
    // result->animation = readAnimationFromFile(dataFilename, malloc(sizeof(Animation)));
    // sprintf(dataFilename, "data/hitboxes/%05d.hitbox", animationId);
    // result->hitboxes = *(readHitboxesFromFile(dataFilename, &(result->hitboxes), 0)); //dumb, but visually consistent
    
    // //if there is no associated animation file, then just load the one for a static animation
    // if (result->animation == NULL){
        // result->animation = readAnimationFromFile("data/animations/no_animation.animation", malloc(sizeof(Animation)));
    // }
    addAnimationToEntity(result, animationId);
    
    return 1;
}

int fillEnemyFromJson(cJSON *root, Enemy *result){
    //fill in the entity superclass
    fillEntityFromJson(cJSON_GetObjectItem(root, "entity"), (Entity *)result);
    
    //set the health
    result->health = cJSON_GetObjectItem(root, "health")->valueint;
    
    //set the damage the player receives for touching
    result->touchDamage = cJSON_GetObjectItem(root, "damage for touching")->valueint;
    
    //load the death entity
    char dataFilename[80];
    int entityId = cJSON_GetObjectItem(root, "death entity")->valueint;
    sprintf(dataFilename, "data/entities/%05d.entity", entityId);
    result->deathEntity = readEntityFromFile(dataFilename, malloc(sizeof(Entity)));
    assignEntityFunctionsById(entityId, result->deathEntity);
    
    return 1;
}

int fillSpriteFromJson(cJSON *root, Sprite *result){
    //load the source image
    char *imageFile = cJSON_GetObjectItem(root, "source image")->valuestring;
    result->image = loadImage(imageFile);
    
    //get the width and height
    result->frameWidth = cJSON_GetObjectItem(root, "frame width")->valueint;
    result->frameHeight = cJSON_GetObjectItem(root, "frame height")->valueint;
    if (result->frameWidth > result->image->width || result->frameWidth <= 0){
        result->frameWidth = result->image->width;
    }
    if (result->frameHeight > result->image->height || result->frameHeight <= 0){
        result->frameHeight = result->image->height;
    }
    
    //compute number of frames per row
    result->numFramesPerRow = result->image->width / result->frameWidth;
    
    return 1;
}

int fillAnimationFromJson(cJSON *root, Animation *result){
    //count loops and allocate
    //PIZZA - should verify that this number and size of lengths array agrees
    int numLoops = cJSON_GetObjectItem(root, "number of loops")->valueint;
    result->numLoops = numLoops;
    result->loopLength = malloc(sizeof(int) * numLoops);
    result->loopTotalDuration = calloc(numLoops, sizeof(int));
    result->repeatLoop = malloc(sizeof(int) * numLoops);
    result->frameNumber = malloc(sizeof(int *) * numLoops);
    result->frameStartTime = malloc(sizeof(int *) * numLoops);
    
    //set the lengths and store loop data
    //PIZZA - as above, need some verification all the numbers align
    size_t i, j;
    int loopLength;
    cJSON *lengthsArr = cJSON_GetObjectItem(root, "loop lengths");
    cJSON *loopsArr = cJSON_GetObjectItem(root, "loops");
    cJSON *loopTemp, *frameArr, *durationArr;
    for (i = 0; i < numLoops; i++){
        loopLength = cJSON_GetArrayItem(lengthsArr, i)->valueint;
        result->loopLength[i] = loopLength;
        result->frameNumber[i] = malloc(sizeof(int) * loopLength);
        result->frameStartTime[i] = malloc(sizeof(int) * loopLength);
        
        //store the loop data
        loopTemp = cJSON_GetArrayItem(loopsArr, i);
        frameArr = cJSON_GetObjectItem(loopTemp, "frame numbers");
        durationArr = cJSON_GetObjectItem(loopTemp, "durations");
        for (j = 0; j < loopLength; j++){
            result->frameNumber[i][j] = cJSON_GetArrayItem(frameArr, j)->valueint;
            result->frameStartTime[i][j] = result->loopTotalDuration[i];
            result->loopTotalDuration[i] += cJSON_GetArrayItem(durationArr, j)->valueint;
        }
        result->repeatLoop[i] = (cJSON_GetObjectItem(loopTemp, "repeat")->type == cJSON_True) ? 1 : 0;
    }
    
    return 1;
}

int fillHitboxesFromJson(cJSON *root, Hitboxes *result){
    size_t i, j, numHitboxes, numShapes;
    cJSON *hitboxArr, *shapeArr, *hitboxJson, *shapeJson;
    
    //movement hitboxes
    hitboxArr = cJSON_GetObjectItem(root, "movement");
    numHitboxes = cJSON_GetArraySize(hitboxArr);
    result->numMovement = numHitboxes;
    result->movement = malloc(sizeof(Hitbox) * numHitboxes);
    for (i = 0; i < numHitboxes; i++){
        hitboxJson = cJSON_GetArrayItem(hitboxArr, i);
        result->movement[i].numCircle = 0;
        result->movement[i].circles = NULL;
        
        shapeArr = cJSON_GetObjectItem(hitboxJson, "rectangles");
        numShapes = cJSON_GetArraySize(shapeArr);
        result->movement[i].numRect = numShapes;
        result->movement[i].rects = malloc(sizeof(CollRect) * numShapes);
        for (j = 0; j < numShapes; j++){
            shapeJson = cJSON_GetArrayItem(shapeArr, j);
            result->movement[i].rects[j].x = cJSON_GetObjectItem(shapeJson, "x")->valueint;
            result->movement[i].rects[j].y = cJSON_GetObjectItem(shapeJson, "y")->valueint;
            result->movement[i].rects[j].w = cJSON_GetObjectItem(shapeJson, "width")->valueint;
            result->movement[i].rects[j].h = cJSON_GetObjectItem(shapeJson, "height")->valueint;
        }
    }
    
    //interact hitboxes
    hitboxArr = cJSON_GetObjectItem(root, "interact");
    numHitboxes = cJSON_GetArraySize(hitboxArr);
    result->numInteract = numHitboxes;
    result->interact = malloc(sizeof(Hitbox) * numHitboxes);
    for (i = 0; i < numHitboxes; i++){
        hitboxJson = cJSON_GetArrayItem(hitboxArr, i);
        result->interact[i].numCircle = 0;
        result->interact[i].circles = NULL;
        
        shapeArr = cJSON_GetObjectItem(hitboxJson, "rectangles");
        numShapes = cJSON_GetArraySize(shapeArr);
        result->interact[i].numRect = numShapes;
        result->interact[i].rects = malloc(sizeof(CollRect) * numShapes);
        for (j = 0; j < numShapes; j++){
            shapeJson = cJSON_GetArrayItem(shapeArr, j);
            result->interact[i].rects[j].x = cJSON_GetObjectItem(shapeJson, "x")->valueint;
            result->interact[i].rects[j].y = cJSON_GetObjectItem(shapeJson, "y")->valueint;
            result->interact[i].rects[j].w = cJSON_GetObjectItem(shapeJson, "width")->valueint;
            result->interact[i].rects[j].h = cJSON_GetObjectItem(shapeJson, "height")->valueint;
        }
    }
    
    return 1;
}

int addAnimationToEntity(Entity *result, int animationId){
    char dataFilename[80];
    sprintf(dataFilename, "data/sprites/%05d.sprite", animationId);
    result->sprite = readSpriteFromFile(dataFilename, malloc(sizeof(Sprite)));
    sprintf(dataFilename, "data/animations/%05d.animation", animationId);
    result->animation = readAnimationFromFile(dataFilename, malloc(sizeof(Animation)));
    sprintf(dataFilename, "data/hitboxes/%05d.hitbox", animationId);
    result->hitboxes = *(readHitboxesFromFile(dataFilename, &(result->hitboxes), 0)); //dumb, but visually consistent
    
    //if there is no associated animation file, then just load the one for a static animation
    if (result->animation == NULL){
        result->animation = readAnimationFromFile("data/animations/no_animation.animation", malloc(sizeof(Animation)));
    }
    
    return 1;
}