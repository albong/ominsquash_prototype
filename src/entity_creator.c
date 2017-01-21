#include "entity_creator.h"
#include "entity.h"
#include "graphics.h"
#include "data_reader.h"
#include "../entities/entity_tables.h"

#include <stdio.h>

static size_t numLoadedEntities = 0;
static size_t *loadedEntitiesIds = NULL;
static Entity **loadedEntities = NULL;

//the function tables
//these types declared in entity_tables.h
//swag : http://stackoverflow.com/a/5488718
static entity_action_ptr_t *entityActionTable; //array
static entity_draw_ptr_t *entityDrawTable; //array
static entity_interact_ptr_t *entityInteractTable; //array
static size_t entityTableSize = 0;

void initEntityCreateTable(){
    // entityCreateTable[0] = &create_entity_00000;
    fillEntityTables(&entityActionTable, &entityDrawTable, &entityInteractTable, &entityTableSize);
}

Entity *createEntityById(size_t id){
    Entity *result = NULL;
    size_t i;
    
    //loop along till we find the correct index, and make a copy of that entity's default
    for (i = 0; i < numLoadedEntities; i++){
        if (loadedEntitiesIds[i] == id){
            result = malloc(sizeof(Entity));
            memcpy(result, loadedEntities[i], sizeof(Entity));
        }
    }
    
    return result;
}

// Sprite *loadEntitySprite(size_t id){
    // Sprite *result;
    
    // switch(id){
        // case SKULL_ID:
            // result = loadAnimatedSprite(SKULL_SPRITE, SKULL_SPRITE_WIDTH);
            // break;
        // default:
            // result = NULL;
            // break;
    // }
    
    // return result;
// }

// Sprite *getLoadedSprite(size_t id){
    // Sprite *result;
    // size_t i;
    // for (i = 0; i < numLoadedSprites; i++){
        // if (loadedSpriteIds[i] == id){
            // result = loadedSprites[i];
            // break;
        // }
    // }
    // return result;
// }

// void loadEntitySprites(size_t *ids, size_t count){
    // numLoadedSprites = count;
    // loadedSpriteIds = malloc(sizeof(size_t) * count);
    // loadedSprites = malloc(sizeof(Sprite *) * count);
    
    // size_t i;
    // for (i = 0; i < count; i++){
        // loadedSpriteIds[i] = ids[i];
        // loadedSprites[i] = loadEntitySprite(ids[i]);
    // }
// }

// void unloadEntitySprites(){
    // size_t i;
    // for (i = 0; i < numLoadedSprites; i++){
        // //need to add code to graphics.h/c to call SDL_FreeSurface on the surface hiding inside the sprite
        // //free(loadedSprites[i]);
    // }
    
    // free(loadedSpriteIds);
    // free(loadedSprites);
    // loadedSpriteIds = NULL;
    // loadedSprites = NULL;
    // numLoadedSprites = 0;
// }

void loadEntityData(size_t *ids, size_t count){
    numLoadedEntities = count;
    loadedEntitiesIds = malloc(sizeof(size_t) * count);
    loadedEntities = malloc(sizeof(Entity *) * count);
    char filename[80];
    
    //load the various entity datas from file and store in our array
    size_t i;
    for (i = 0; i < count; i++){
        loadedEntitiesIds[i] = ids[i];
        sprintf(filename, "data/entities/%05d.entity", ids[i]);
        loadedEntities[i] = readEntityFromFile(filename, malloc(sizeof(Entity)));
        
        //set the entity's methods from the tables
        if (entityActionTable[ids[i]] != NULL){
            loadedEntities[i]->action = entityActionTable[ids[i]];
        }
        if (entityDrawTable[ids[i]] != NULL){
            loadedEntities[i]->draw = entityDrawTable[ids[i]];
        }
        if (entityActionTable[ids[i]] != NULL){
            loadedEntities[i]->interact = entityInteractTable[ids[i]];
        }
    }
}

void unloadEntityData(){
    
}