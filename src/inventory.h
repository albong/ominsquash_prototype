#ifndef INVENTORY_H
#define INVENTORY_H

#include "weapon.h"

/*
I kinda think it might be better to always have EVERYTHING in the inventory, with
a flag for whether or not its actually in the inventory.

We def need to ask Vince about this idea
*/

// Does this really need to be a struct?  Couldn't we just store all this
// data in private variables in the C file with accessor methods?
// Its not like anyone but the player will have an inventory.
// I guess it might be good to pass this stuff about?  Maybe?
// typedef struct Inventory {
    // int numWeapons;
    // Weapon **weaponList; //i guess weapons also need icons now
    // int numKeyLists; //corresponds to the number of areas
    // Keys ***keyLists; //a list of lists of keys, the index in the original list 
                    // //is what area each set of keys belong to - maybe a struct for these instead?
    // int *numKeys; // the number of keys in each list in the above list
    // int numItems;
    // Item ** itemList;
// } Inventory;

void initInventory();
void termInventory();
void addWeaponToInventory(Weapon *w);

#endif