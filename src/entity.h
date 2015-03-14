#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

#define MAX_ENTITIES 50

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Orientation;

typedef struct Entity {
    int active;
    double pixelsPerMilli;
    double x, y;
	int w, h;
	int isMoving;
	int milliPerFrame, milliPassed;
	int numFrames;
	Sprite *sprite;
	Orientation orientation;
	void (*action)(void);
	void (*draw)(void);
} Entity;

Entity entity[MAX_ENTITIES];
Entity *self;

void clearEntities();
int getFreeEntity();
void doEntities();
void drawEntities();
void drawStandardEntity();


#endif
