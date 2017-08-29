#ifndef DATA_READER_H
#define DATA_READER_H

#include "area.h"
#include "entity.h"
#include "graphics.h"
#include "hitbox.h"

Area *readAreaFromFile(char *filename, Area *result);
Entity *readEntityFromFile(char *filename, Entity *result);
Enemy *readEnemyFromFile(char *filename, Enemy *result);
Sprite *readSpriteFromFile(char *filename, Sprite *result);
Animation *readAnimationFromFile(char *filename, Animation *result);
Hitboxes *readHitboxesFromFile(char *filename, Hitboxes *result, int allowAllocate);
int readAnimationIntoEntity(Entity *result, int animationId); //returns 0 on fail, 1 on success

#endif
