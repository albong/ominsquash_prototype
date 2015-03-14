#ifndef MAIN_H
#define MAIN_H

#include "structs.h"
#include "entity.h"

Game game;
Control input;
Entity player, *self, entity[MAX_ENTITIES];
Sprite sprite[MAX_SPRITES];
Sound sound[MAX_SOUNDS];
Star star[MAX_STARS];
Animation animation[MAX_ANIMATIONS];

#endif
