#include "enemy_creator.h"
#include "enemy.h"
#include "graphics.h"
#include "data_reader.h"
#include "../enemies/enemy_tables.h"

#include <stdio.h>

static size_t numLoadedEnemies = 0;
static size_t *loadedEnemiesIds = NULL;
static Enemy **loadedEnemies = NULL;

//the function tables
//these types declared in enemy_tables.h
//swag : http://stackoverflow.com/a/5488718
static enemy_entity_construct_ptr_t *eConstructTable; //array
static enemy_entity_destruct_ptr_t *eDestructTable; //array
static enemy_entity_action_ptr_t *eActionTable; //array
static enemy_draw_ptr_t *drawTable; //array
static enemy_interact_ptr_t *interactTable; //array
static enemy_takeDamage_ptr_t *takeDamageTable; //array
static enemy_collidePlayer_ptr_t *collidePlayerTable; //array
static enemy_action_ptr_t *actionTable; //array
static size_t enemyTableSize = 0;

void initEnemyCreateTable(){
    fillEnemyTables(&eConstructTable, &eDestructTable, &eActionTable, &drawTable, &interactTable, &takeDamageTable, &collidePlayerTable, &actionTable, &enemyTableSize);
}

void termEnemyCreateTable(){
    free(eConstructTable);
    free(eDestructTable);
    free(eActionTable);
    free(drawTable);
    free(interactTable);
    free(takeDamageTable);
    free(collidePlayerTable);
    free(actionTable);    
}

Enemy *createEnemyById(size_t id){
    Enemy *result = NULL;
    size_t i;
    
    //loop along till we find the correct index, and make a copy of that enemy's default
    for (i = 0; i < numLoadedEnemies; i++){
        if (loadedEnemiesIds[i] == id){
            result = malloc(sizeof(Enemy));
            memcpy(result, loadedEnemies[i], sizeof(Enemy));
            result->e.animation = shallowCopyAnimation(loadedEnemies[i]->e.animation);
            if (result->e.construct != NULL){
                result = result->e.construct((Entity *)result); //and then we immediately recast it in the method
            }
            break;
        }
    }
    
    return result;
}

Enemy *assignEnemyFunctionsById(size_t id, Enemy *e){
    if (e == NULL){
        return NULL;
    }
    
    //set the entity's methods from the tables
    if (eConstructTable[id] != NULL){
        e->e.construct = eConstructTable[id];
    }
    if (eDestructTable[id] != NULL){
        e->e.destruct = eDestructTable[id];
    }
    if (eActionTable[id] != NULL){
        e->e.action = eActionTable[id];
    }
    if (drawTable[id] != NULL){
        e->e.draw = drawTable[id];
    }
    if (interactTable[id] != NULL){
        e->e.interact = interactTable[id];
    }
    if (takeDamageTable[id] != NULL){
        e->takeDamage = takeDamageTable[id];
    }
    if (collidePlayerTable[id] != NULL){
        e->collidePlayer = collidePlayerTable[id];
    }
    if (actionTable[id] != NULL){
        e->action = actionTable[id];
    }
    
    return e;
}

void loadEnemyData(size_t *ids, size_t count){
    numLoadedEnemies = count;
    loadedEnemiesIds = malloc(sizeof(size_t) * count);
    loadedEnemies = malloc(sizeof(Enemy *) * count);
    char filename[FILENAME_BUFFER_SIZE];
    
    //load the various datas from file and store in our array
    size_t i;
    for (i = 0; i < count; i++){
        loadedEnemiesIds[i] = ids[i];
        sprintf(filename, "data/enemies/%05d.enemy", ids[i]);
        loadedEnemies[i] = readEnemyFromFile(filename, NULL);
        
        //set the methods from the tables
        assignEntityFunctionsById(ids[i], loadedEnemies[i]);
        
        //construct - doesn't really matter, since this gets shallow copied, except that destruct may expect things to be allocated
        if (loadedEnemies[i]->e.construct != NULL){
            loadedEnemies[i] = loadedEnemies[i]->e.construct((Entity *)loadedEnemies[i]);
        }
    }
}

void unloadEnemyData(){
    size_t i;
    for (i = 0; i < numLoadedEnemies; i++){
        free_Enemy(loadedEnemies[i]);
        loadedEnemies[i] = NULL;
    }
    free(loadedEnemies);
    free(loadedEnemiesIds);
    loadedEnemiesIds = NULL;
    loadedEnemies = NULL;
    numLoadedEnemies = 0;
}
