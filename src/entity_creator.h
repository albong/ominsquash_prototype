#ifndef ENTITY_CREATOR_H
#define ENTITY_CREATOR_H

#include "entity.h"

void initEntityCreateTable();
Entity *createEntityById(size_t id);
Entity *assignEntityFunctionsById(size_t id, Entity *e);

//PIZZA - should change this to loadEntityData(...), load and pass off the data files, and from these
// load the sprite info - probably should generalize to a subclass of interactables or something

void loadEntityData(size_t *ids, size_t count);
// void loadEntitySprites(size_t *ids, size_t count);
void unloadEntityData();
// void unloadEntitySprites();

#endif
