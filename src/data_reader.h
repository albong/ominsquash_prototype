#ifndef DATA_READER_H
#define DATA_READER_H

#include "area.h"
#include "entity.h"
#include "graphics.h"
#include "hitbox.h"
#include "text.h"
#include "sound.h"
#include "cutscene.h"

// NOTE: It is unsafe to pass in memory that cannot be freed to these functions.
// If the file read or JSON parsing fails, then the methods attempt to free the
// "result" memory.  This is fine (and desired) in the case that result is NULL
// or is allocated prior to passing in.  But if the memory cannot be freed, either
// because it is part of an array, or because you have passed the address to static
// memory, there will be problems.
//
// Probably we should investigate whether or not this should continue - adding
// a flag to only free if the memory was also allocated in the function is
// trivial, but we'd need to check where all of these are called.

Area *readAreaFromFile(char *filename, Area *result);
Entity *readEntityFromFile(char *filename, Entity *result);
Enemy *readEnemyFromFile(char *filename, Enemy *result);
Sprite *readSpriteFromFile(char *filename, Sprite *result);
Animation *readAnimationFromFile(char *filename, Animation *result);
Hitboxes *readHitboxesFromFile(char *filename, Hitboxes *result, int allowAllocate);
int readAnimationIntoEntity(Entity *result, int animationId); //returns 0 on fail, 1 on success
Text *readTextFromFile(char *filename, Text *result);
Music *readMusicFromFile(char *filename, Music *result);
Sound *readSoundFromFile(char *filename, Sound *result);
Cutscene *readCutsceneFromFile(char *filename, Cutscene *result);

#endif
