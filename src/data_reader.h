#ifndef DATA_READER_H
#define DATA_READER_H

#include "area.h"
#include "entity.h"
#include "graphics.h"

Area *readAreaFromFile(char *filename, Area *result);
Entity *readEntityFromFile(char *filename, Entity *result);
Enemy *readEnemyFromFile(char *filename, Enemy *result);
Sprite *readSpriteFromFile(char *filename, Sprite *result);
SpriteAnimation *readSpriteAnimationFromFile(char *filename, SpriteAnimation *result);

#endif
