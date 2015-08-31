#ifndef ENEMY_LOADER_H
#define ENEMY_LOADER_H

#include "../src/enemy.h"

Enemy *createEnemyById(size_t id);
void loadEnemySprites(size_t *ids, size_t count);
void unloadEnemySprites();

#endif
