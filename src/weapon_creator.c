#include "weapon_creator.h"
#include "weapon.h"
#include "graphics.h"
#include "data_reader.h"
#include "../weapons/weapon_tables.h"

#include <stdio.h>

// static size_t numLoadedWeapons = 0;
// static size_t *loadedWeaponIds = NULL;
// static Weapon **loadedWeapons = NULL;

//the function tables
//these types declared in entity_tables.h
//swag : http://stackoverflow.com/a/5488718
static weapon_create_ptr_t *weaponCreateTable; //array
static weapon_collide_ptr_t *weaponCollideTable; //array
static size_t weaponTableSize = 0;

void initWeaponCreateTable(){
    fillWeaponTables(&weaponCreateTable, &weaponCollideTable, &weaponTableSize);
}

Weapon *createWeaponById(size_t id){
    if (id >= weaponTableSize){
        return NULL;
    }
    
    Weapon *result = NULL;
    char filename[80];
    
    //ultimately we'll load from file, just hack it for now
    // sprintf(filename, "data/entities/%05d.entity", id);
    result = weaponCreateTable[id]();
    
    return result;
}

// void loadEntityData(size_t *ids, size_t count){
    // numLoadedEntities = count;
    // loadedEntitiesIds = malloc(sizeof(size_t) * count);
    // loadedEntities = malloc(sizeof(Entity *) * count);
    // char filename[80];
    
    // //load the various entity datas from file and store in our array
    // size_t i;
    // for (i = 0; i < count; i++){
        // loadedEntitiesIds[i] = ids[i];
        // sprintf(filename, "data/entities/%05d.entity", ids[i]);
        // loadedEntities[i] = readEntityFromFile(filename, malloc(sizeof(Entity)));
        
        // //set the entity's methods from the tables
        // if (entityActionTable[ids[i]] != NULL){
            // loadedEntities[i]->action = entityActionTable[ids[i]];
        // }
        // if (entityDrawTable[ids[i]] != NULL){
            // loadedEntities[i]->draw = entityDrawTable[ids[i]];
        // }
        // if (entityInteractTable[ids[i]] != NULL){
            // loadedEntities[i]->interact = entityInteractTable[ids[i]];
        // }
    // }
// }

void unloadWeaponData(){
    //PIZZA - Implement me!
}