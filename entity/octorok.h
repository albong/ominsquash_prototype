#ifndef OCTOROK_H
#define OCTOROK_H

#include "../src/Entity.h"

Entity *createOctorok(int spriteIndex);
static void doOctorok(int delta, Entity *e);
static void updatePosition(int delta, Entity *e);
static void move(Entity *e);
#endif
