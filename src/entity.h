#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"
#include "collisions.h"

#define MAX_ENTITIES 50

typedef enum {
    UP, DOWN, LEFT, RIGHT
} Orientation;

typedef enum {
    NONE, PLAYER, ENEMY, ENEMY_COLL, WALL, PROJECTILE, OBJECT
} CollisionType; //ENEMY_COLL collides with other enemies

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
	void (*action)(struct Entity*, int);
	void (*draw)(struct Entity*, double, double);
	void (*collide)(struct Entity*);
	CollisionType type;
	HitBox *moveHitBox;
	HitBox *interactHitBox;
} Entity;

void moveEntity(Entity *e);
void collideWithWallX(CollRect wall, Entity *e, CollRect r, int collCode);
void collideWithWallY(CollRect wall, Entity *e, CollRect r, int collCode);

#endif
