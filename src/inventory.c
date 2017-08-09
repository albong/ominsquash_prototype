#include "inventory.h"
#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>

/////////////////////////////////////////////////
// Data
/////////////////////////////////////////////////
static size_t numWeapons;
static Weapon **weaponList;
// int numKeyLists; //corresponds to the number of areas
// Keys ***keyLists; //a list of lists of keys, the index in the original list 
                // //is what area each set of keys belong to - maybe a struct for these instead?
// int *numKeys; // the number of keys in each list in the above list
// int numItems;
// Item ** itemList;

/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
void initInventory(){
    numWeapons = 0;
    weaponList = NULL;
}

void termInventory(){
    //the weapons themselves ought already have been freed
    free(weaponList);
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////

void addWeaponToInventory(Weapon *w){
    numWeapons++;
    weaponList = realloc(weaponList, sizeof(Weapon *) * numWeapons);
    weaponList[numWeapons - 1] = w;
}