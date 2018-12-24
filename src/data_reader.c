#include "data_reader.h"
#include "file_reader.h"
#include "area.h"
#include "room.h"
#include "stair.h"
#include "constants.h"
#include "entity.h"
#include "hitbox.h"
#include "graphics.h"
#include "entity_creator.h"
#include "sound.h"
#include "logging.h"
#include "cutscene.h"

#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//PIZZA - Needs to check for existence of all fields being read!


//return 0 on failure
static int fillAreaFromJson(cJSON *root, Area *result);
static int fillEntityFromJson(cJSON *root, Entity *result);
static int fillEnemyFromJson(cJSON *root, Enemy *result);
static int fillSpriteFromJson(cJSON *root, Sprite *result);
static int fillAnimationFromJson(cJSON *root, Animation *result);
static int fillHitboxesFromJson(cJSON *root, Hitboxes *result);
static int fillLinearTweenFromJson(cJSON *root, LinearTween *result);

Area *readAreaFromFile(char *filename, Area *result){
    //init the area
    if (result == NULL){
        result = malloc(sizeof(Area));
    }
    init_Area(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToString(filename);
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
    char *fileContents = readFileToString(filename);
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
    char *fileContents = readFileToString(filename);
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
    char *fileContents = readFileToString(filename);
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
    char *fileContents = readFileToString(filename);
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
    char *fileContents = readFileToString(filename);
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
            
            tempStair->toArea = cJSON_GetObjectItem(jsonTemp, "to area")->valueint;
            tempStair->toRoom = cJSON_GetObjectItem(jsonTemp, "to room")->valueint;
            tempStair->toX = cJSON_GetObjectItem(jsonTemp, "to x")->valueint;
            tempStair->toY = cJSON_GetObjectItem(jsonTemp, "to y")->valueint;
            
            if (tempStair->toArea == -1){
                tempStair->sameArea = 1;
            }
            
            animationId = cJSON_GetObjectItem(jsonTemp, "animation")->valueint;
            readAnimationIntoEntity((Entity *)tempStair, animationId);
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
    // char dataFilename[FILENAME_BUFFER_SIZE];
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
    readAnimationIntoEntity(result, animationId);
    
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
    char dataFilename[FILENAME_BUFFER_SIZE];
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

int readAnimationIntoEntity(Entity *result, int animationId){
    if (result == NULL){
        return 0;
    }
    
    char dataFilename[FILENAME_BUFFER_SIZE];
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

Music *readMusicFromFile(char *filename, Music *result){
    if (result == NULL){
        result = malloc(sizeof(Music));
    }
    init_Music(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToString(filename);
    cJSON *root = cJSON_Parse(fileContents);
    
    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root){
        LOG_ERR("%s had a JSON error: %s", filename, cJSON_GetErrorPtr());
        free(result);
        result = NULL;
    } else {
        //open the sound file
        char *musicFile = cJSON_GetObjectItem(root, "music file")->valuestring;
        fillMusicFromFile(result, musicFile);
        
        //set the volume
        result->volumeAdjust = cJSON_GetObjectItem(root, "volume adjust")->valueint;
        
        //free
        cJSON_Delete(root);
    }
    
    //free and return
    free(fileContents);
    LOG_INF("Music at %s read into %p", filename, result);
    return result;
}

Sound *readSoundFromFile(char *filename, Sound *result){
    if (result == NULL){
        result = malloc(sizeof(Sound));
    }
    init_Sound(result);

    //read in the given file to a cJSON object
    char *fileContents = readFileToString(filename);
    cJSON *root = cJSON_Parse(fileContents);
    
    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root){
        LOG_ERR("%s had a JSON error: %s", filename, cJSON_GetErrorPtr());
        free(result);
        result = NULL;
    } else {
        //open the sound file
        char *soundFile = cJSON_GetObjectItem(root, "sound file")->valuestring;
        fillSoundFromFile(result, soundFile);
        
        //set the volume
        result->volumeAdjust = cJSON_GetObjectItem(root, "volume adjust")->valueint;
        
        //free
        cJSON_Delete(root);
    }
    
    //free and return
    free(fileContents);
    LOG_INF("Sound at %s read into %p", filename, result);
    return result;
}

Cutscene *readCutsceneFromFile(char *filename, Cutscene *result){
    if (result == NULL){
        result = malloc(sizeof(Cutscene));
    }
    init_Cutscene(result);
    
    //read in the given file to a cJSON object
    char *fileContents = readFileToString(filename);
    cJSON *root = cJSON_Parse(fileContents);
    
    //if the initial parsing failed, or the detailed parsing failed, print an error and free/NULL result
    //otherwise free the cJSON root correctly
    if (!root){
        LOG_ERR("%s had a JSON error: %s", filename, cJSON_GetErrorPtr());
        free(result);
        result = NULL;
    } else {
        cJSON *array, *arrayItem, *tweenArray;
        size_t i, j;
        char dataFilename[FILENAME_BUFFER_SIZE];
        
        //player tweens
        array = cJSON_GetObjectItem(root, "player tweens");
        result->numPlayerTweens = cJSON_GetArraySize(array);
        result->playerTweens = malloc(sizeof(LinearTween *) * result->numPlayerTweens);
        for (i = 0; i < result->numPlayerTweens; i++){
            result->playerTweens[i] = init_LinearTween(malloc(sizeof(LinearTween)));
            fillLinearTweenFromJson(cJSON_GetArrayItem(array, i), result->playerTweens[i]);
        }
        
        //entities
        array = cJSON_GetObjectItem(root, "entity animations");
        result->numEntities = cJSON_GetArraySize(array);
        result->entities = malloc(sizeof(Entity *) * result->numEntities);
        result->numEntityTweens = malloc(sizeof(int) * result->numEntities);
        result->entityTweens = malloc(sizeof(LinearTween **) * result->numEntities);
        for (i = 0; i < result->numEntities; i++){
            arrayItem = cJSON_GetArrayItem(array, i);
            tweenArray = cJSON_GetObjectItem(arrayItem, "tweens");
            
            //read the entity from file
            sprintf(dataFilename, "data/entities/%05d.entity", cJSON_GetObjectItem(arrayItem, "entity id")->valueint);
            result->entities[i] = readEntityFromFile(dataFilename, malloc(sizeof(Entity)));
            
            //gotta fill an array of pointers for each entity
            result->numEntityTweens[i] = cJSON_GetArraySize(tweenArray);
            result->entityTweens[i] = malloc(sizeof(LinearTween *) * result->numEntityTweens[i]);
            for (j = 0; j < result->numEntityTweens[i]; i++){
                result->entityTweens[i][j] = init_LinearTween(malloc(sizeof(LinearTween)));
                fillLinearTweenFromJson(cJSON_GetArrayItem(tweenArray, j), result->entityTweens[i][j]);
            }
        }
        
        //sounds
        array = cJSON_GetObjectItem(root, "sounds");
        result->numSounds = cJSON_GetArraySize(array);
        result->sounds = malloc(sizeof(Sound *) * result->numSounds);
        result->timeToPlay = malloc(sizeof(int) * result->numSounds);
        result->numRepeats = malloc(sizeof(int) * result->numSounds);
        for (i = 0; i < result->numSounds; i++){
            arrayItem = cJSON_GetArrayItem(array, i);
            
            //read the sound file
            sprintf(dataFilename, "data/sound/%05d.sound", cJSON_GetObjectItem(arrayItem, "sound id")->valueint);
            result->sounds[i] = readSoundFromFile(dataFilename, malloc(sizeof(Sound)));
            
            //fill the other data
            result->timeToPlay[i] = cJSON_GetObjectItem(arrayItem, "play at time")->valueint;
            result->numRepeats[i] = cJSON_GetObjectItem(arrayItem, "number of repeats")->valueint;
        }
        
        //music
        array = cJSON_GetObjectItem(root, "music");
        result->numMusic = cJSON_GetArraySize(array);
        result->music = malloc(sizeof(Music *) * result->numMusic);
        result->musicStartTime = malloc(sizeof(int) * result->numMusic);
        result->musicEndTime = malloc(sizeof(int) * result->numMusic);
        result->fadeInDuration = malloc(sizeof(int) * result->numMusic);
        result->fadeOutDuration = malloc(sizeof(int) * result->numMusic);
        for (i = 0; i < result->numMusic; i++){
            arrayItem = cJSON_GetArrayItem(array, i);
            
            //read the music file
            sprintf(dataFilename, "data/music/%05d.music", cJSON_GetObjectItem(arrayItem, "music id")->valueint);
            result->music[i] = readMusicFromFile(dataFilename, malloc(sizeof(Music)));
            
            //fill the other data
            result->musicStartTime[i] = cJSON_GetObjectItem(arrayItem, "start at time")->valueint;
            result->musicEndTime[i] = cJSON_GetObjectItem(arrayItem, "end at time")->valueint;
            result->fadeInDuration[i] = cJSON_GetObjectItem(arrayItem, "fade in duration")->valueint;
            result->fadeOutDuration[i] = cJSON_GetObjectItem(arrayItem, "fade out duration")->valueint;
        }
        
        //text
        array = cJSON_GetObjectItem(root, "text");
        result->numText = cJSON_GetArraySize(array);
        result->text = malloc(sizeof(Text *) * result->numText);
        result->showTextAtTime = malloc(sizeof(int) * result->numText);
        for (i = 0; i < result->numText; i++){
            arrayItem = cJSON_GetArrayItem(array, i);
            
            //read the text file in the correct language
            sprintf(dataFilename, "data/text/%s/%05d.text", getCurrentLanguageString(), cJSON_GetObjectItem(arrayItem, "text id")->valueint);
            //result->text[i] = readTextFromFile(dataFilename, malloc(sizeof(Text)));
            
            //fill the other data
            result->showTextAtTime[i] = cJSON_GetObjectItem(arrayItem, "show at time")->valueint;
        }
        
        //free
        cJSON_Delete(root);
    }
    
    //free and return
    free(fileContents);
    LOG_INF("Cutscene at %s read into %p", filename, result);
    return result;
}

int fillLinearTweenFromJson(cJSON *root, LinearTween *result){
    if (cJSON_GetObjectItem(root, "draw")->type == cJSON_True){
        result->draw = 1;
    }
    
    result->startX = cJSON_GetObjectItem(root, "start x")->valuedouble;
    result->startY = cJSON_GetObjectItem(root, "start y")->valuedouble;
    
    if (cJSON_GetObjectItem(root, "is moving")->type == cJSON_True){
        result->isMoving = 1;
    }
    
    result->endX = cJSON_GetObjectItem(root, "end x")->valuedouble;
    result->endY = cJSON_GetObjectItem(root, "end y")->valuedouble;
    
    result->animationLoop = cJSON_GetObjectItem(root, "animation loop")->valueint;
    
    result->moveDuration = cJSON_GetObjectItem(root, "duration")->valueint;
    
    return 1;
}


