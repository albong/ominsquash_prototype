#ifndef ENTITY_H
#define ENTITY_H

#include "defs.h"
#include "structs.h"

#define MAX_ENTITIES 50

typedef struct Entity {
	int active, speed;
	int x, y, w, h;
	int currentFrame, animID, frameTimer;
	SDL_Surface *sprite;
	void (*action)(void);
	void (*draw)(void);
	Orientation orientation;
} Entity;

extern Entity *self, entity[MAX_ENTITIES];
extern Animation animation[MAX_ANIMATIONS];

void clearEntities();
int getFreeEntity();
void doEntities();
void drawEntities();
void drawStandardEntity();
void drawAnimatedEntity();

#endif
