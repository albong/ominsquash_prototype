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
    NONE, PLAYER, ENEMY, ENEMY_COLL, ARMOR, WALL, WEAPON, OBJECT, DOOR
} CollisionType; //ENEMY_COLL collides with other enemies

// Structs
typedef struct EntityExtMove {
    double delX, delY;
    double distance;
    double velocity;
} EntityExtMove;

typedef struct Entity {
    int active;
    double pixelsPerMilli;
    double x, y;
    double startX, startY;
    double changeX, changeY;
    size_t numExternalMove;
    EntityExtMove *externalMove;
	int w, h;
	int isMoving;
	int milliPerFrame, milliPassed;
	int currFrame;
	int numFrames;
	int invertSprite;
	Sprite *sprite;
	Orientation orientation;
	void (*action)(void *, int);
	void (*draw)(struct Entity *, double shiftX, double shiftY);
//	void (*collide)(void *);
	CollisionType type;
	int currHitBox;
	int hasMoveHitBox;
	int hasInteractHitBox;
	HitBox *moveHitBox;//array
	HitBox *interactHitBox;//array
} Entity;

// Loading
Entity *init_Entity(Entity *self);

// Logic
void moveEntity(Entity *e);
void collideWithWallX(CollRect wall, Entity *e, CollRect r, int collCode);
void collideWithWallY(CollRect wall, Entity *e, CollRect r, int collCode);
void addExternalMove(Entity *self, double x, double y, double magnitude, double velocity);
void applyExternalMoves(Entity *self, int delta);

// Drawing
//static void drawEntity

#endif
