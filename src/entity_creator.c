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
static entity_construct_ptr_t *entityConstructTable; //array
static entity_destruct_ptr_t *entityDestructTable; //array
static entity_action_ptr_t *entityActionTable; //array
static entity_draw_ptr_t *entityDrawTable; //array
static entity_interact_ptr_t *entityInteractTable; //array
static size_t entityTableSize = 0;

void initEntityCreateTable(){
    fillEntityTables(&entityConstructTable, &entityDestructTable, &entityActionTable, &entityDrawTable, &entityInteractTable, &entityTableSize);
}

void termEntityCreateTable(){
    free(entityConstructTable);
    free(entityDestructTable);
    free(entityActionTable);
    free(entityDrawTable);
    free(entityInteractTable);
}

Entity *createEntityById(size_t id){
    Entity *result = NULL;
    size_t i;
    
    //loop along till we find the correct index, and make a copy of that entity's default
    for (i = 0; i < numLoadedEntities; i++){
        if (loadedEntitiesIds[i] == id){
            result = malloc(sizeof(Entity));
            memcpy(result, loadedEntities[i], sizeof(Entity));
            result->animation = shallowCopyAnimation(loadedEntities[i]->animation);
            if (result->construct != NULL){
                result->construct(result);
            }
            break;
        }
    }
    
    return result;
}

Entity *assignEntityFunctionsById(size_t id, Entity *e){
    if (e == NULL){
        return NULL;
    }
    
    //set the entity's methods from the tables
    if (entityConstructTable[id] != NULL){
        e->construct = entityConstructTable[id];
    }
    if (entityDestructTable[id] != NULL){
        e->destruct = entityDestructTable[id];
    }
    if (entityActionTable[id] != NULL){
        e->action = entityActionTable[id];
    }
    if (entityDrawTable[id] != NULL){
        e->draw = entityDrawTable[id];
    }
    if (entityInteractTable[id] != NULL){
        e->interact = entityInteractTable[id];
    }
    
    return e;
}

void loadEntityData(size_t *ids, size_t count){
    numLoadedEntities = count;
    loadedEntitiesIds = malloc(sizeof(size_t) * count);
    loadedEntities = malloc(sizeof(Entity *) * count);
    char filename[FILENAME_BUFFER_SIZE];
    
    //load the various entity datas from file and store in our array
    size_t i;
    for (i = 0; i < count; i++){
        loadedEntitiesIds[i] = ids[i];
        sprintf(filename, "data/entities/%05d.entity", ids[i]);
        loadedEntities[i] = readEntityFromFile(filename, malloc(sizeof(Entity)));
        
        //set the entity's methods from the tables
        assignEntityFunctionsById(ids[i], loadedEntities[i]);
    }
}

void unloadEntityData(){
    size_t i;
    for (i = 0; i < numLoadedEntities; i++){
        free_Entity(loadedEntities[i]);
        loadedEntities[i] = NULL;
    }
    free(loadedEntities);
    free(loadedEntitiesIds);
    loadedEntitiesIds = NULL;
    loadedEntities = NULL;
    numLoadedEntities = 0;
}
