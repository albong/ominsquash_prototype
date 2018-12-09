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
    Animation *animation;
	Orientation orientation;
    void *(*construct)(struct Entity *self);
    void (*destruct)(struct Entity *self);
	void (*action)(void *, int); //probs should be Entity *, but whatevs? - The advantage of void is not having to cast when calling, despite having to cast in method, especially when subclassing
	void (*draw)(struct Entity *, double shiftX, double shiftY);
	void (*collidePlayer)(struct Entity *, int collisionCode);
	CollisionType type;
    Hitboxes hitboxes;
	int interactable; //when we move to single hitboxes and collision groups, this will be replaced by hasInteractHitbox
	void (*interact)(struct Entity *self);
} Entity;

// Loading/Unloading
Entity *init_Entity(Entity *self);
void term_Entity(Entity *self);
void free_Entity(Entity *self);

// Logic
void moveEntity(Entity *e);
void collideWithWallX(CollRect wall, Entity *e, CollRect r, int collCode);
void collideWithWallY(CollRect wall, Entity *e, CollRect r, int collCode);
void addExternalMove(Entity *self, double x, double y, double magnitude, double velocity);
void addImpactMove(Entity *self, double xFrom, double yFrom, double magnitude, double velocity);
void applyExternalMoves(Entity *self, int delta);

// Drawing
//static void drawEntity

#endif
