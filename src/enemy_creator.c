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
static enemy_entity_action_ptr_t *eActionTable; //array
static enemy_draw_ptr_t *drawTable; //array
static enemy_interact_ptr_t *interactTable; //array
static enemy_takeDamage_ptr_t *takeDamageTable; //array
static enemy_collidePlayer_ptr_t *collidePlayerTable; //array
static enemy_action_ptr_t *actionTable; //array
static size_t enemyTableSize = 0;

void initEnemyCreateTable(){
    fillEnemyTables(&eActionTable, &drawTable, &interactTable, &takeDamageTable, &collidePlayerTable, &actionTable, &enemyTableSize);
}

Enemy *createEnemyById(size_t id){
    Enemy *result = NULL;
    size_t i;
    
    //loop along till we find the correct index, and make a copy of that enemy's default
    for (i = 0; i < numLoadedEnemies; i++){
        if (loadedEnemiesIds[i] == id){
            result = malloc(sizeof(Enemy));
            memcpy(result, loadedEnemies[i], sizeof(Enemy));
            result->e.animation = shallowCopySpriteAnimation(loadedEnemies[i]->e.animation);
        }
    }
    
    return result;
}

void loadEnemyData(size_t *ids, size_t count){
    numLoadedEnemies = count;
    loadedEnemiesIds = malloc(sizeof(size_t) * count);
    loadedEnemies = malloc(sizeof(Enemy *) * count);
    char filename[80];
    
    //load the various datas from file and store in our array
    size_t i;
    for (i = 0; i < count; i++){
        loadedEnemiesIds[i] = ids[i];
        sprintf(filename, "data/enemies/%05d.enemy", ids[i]);
        loadedEnemies[i] = readEnemyFromFile(filename, NULL);
        
        //set the methods from the tables
        if (eActionTable[ids[i]] != NULL){
            loadedEnemies[i]->e.action = eActionTable[ids[i]];
        }
        if (drawTable[ids[i]] != NULL){
            loadedEnemies[i]->e.draw = drawTable[ids[i]];
        }
        if (interactTable[ids[i]] != NULL){
            loadedEnemies[i]->e.interact = interactTable[ids[i]];
        }
        if (takeDamageTable[ids[i]] != NULL){
            loadedEnemies[i]->takeDamage = takeDamageTable[ids[i]];
        }
        if (collidePlayerTable[ids[i]] != NULL){
            loadedEnemies[i]->collidePlayer = collidePlayerTable[ids[i]];
        }
        if (actionTable[ids[i]] != NULL){
            loadedEnemies[i]->action = actionTable[ids[i]];
        }
    }
}

void unloadEnemyData(){
    //PIZZA - Implement me!
}