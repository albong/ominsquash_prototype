#ifndef ENEMY_CREATOR_H
#define ENEMY_CREATOR_H

#include "enemy.h"

void initEnemyCreateTable();
void termEnemyCreateTable();
Enemy *createEnemyById(size_t id);
Enemy *assignEnemyFunctionsById(size_t id, Enemy *e);

//PIZZA - should change this to loadEntityData(...), load and pass off the data files, and from these
// load the sprite info - probably should generalize to a subclass of interactables or something

void loadEnemyData(size_t *ids, size_t count);
// void loadEntitySprites(size_t *ids, size_t count);
void unloadEnemyData();
// void unloadEntitySprites();

#endif
