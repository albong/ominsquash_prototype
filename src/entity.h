#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"
#include "collisions.h"

#define MAX_ENTITIES 50

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Orientation;

typedef enum {
    NONE, PLAYER, ENEMY, WALL, PROJECTILE, OBJECT
} CollisionType;

typedef struct Entity {
    int active;
    double pixelsPerMilli;
    double x, y;
    double changeX, changeY;
	int w, h;
	int isMoving;
	int milliPerFrame, milliPassed;
	int numFrames;
	Sprite *sprite;
	Orientation orientation;
	void (*action)(void);
	void (*draw)(void);
	void (*collide)(struct Entity*);
	CollisionType type;
	HitBox *moveHitBox;
	HitBox *interactHitBox;
} Entity;

Entity entity[MAX_ENTITIES];
Entity *self;

void clearEntities();
int getFreeEntity();
void doEntities();
void drawEntities();
void drawStandardEntity();
void moveEntities();
void collideWithWall(CollRect wall, Entity e, int collCode);

#endif
