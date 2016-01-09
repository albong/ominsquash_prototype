#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include "../src/entity.h"

Entity *createEntityById(size_t id);
void loadEntitySprites(size_t *ids, size_t count);
void unloadEntitySprites();

#endif
