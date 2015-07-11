#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"
#include "hitbox.h"

#define MAX_ENTITIES 50

// Enums
typedef enum {
    UP, DOWN, LEFT, RIGHT
} Orientation;

typedef enum {
    NONE, PLAYER, ENEMY, ENEMY_COLL, ARMOR, WALL, WEAPON, OBJECT
} CollisionType; //ENEMY_COLL collides with other enemies

// Structs
typedef struct Entity {
    int active;
    double pixelsPerMilli;
    double x, y;
    double startX, startY;
    double changeX, changeY;
	int w, h;
	int isMoving;
	int milliPerFrame, milliPassed;
	int currFrame;
	int numFrames;
	Sprite *sprite;
	Orientation orientation;
	void (*action)(void *, int);
	void (*draw)(struct Entity *, double, double);
//	void (*collide)(void *);
	CollisionType type;
	int currHitBox;
	int hasMoveHitBox;
	int hasInteractHitBox;
	HitBox *moveHitBox;//array
	HitBox *interactHitBox;//array
} Entity;

// Logic
void moveEntity(Entity *e);
void collideWithWallX(CollRect wall, Entity *e, CollRect r, int collCode);
void collideWithWallY(CollRect wall, Entity *e, CollRect r, int collCode);

#endif
