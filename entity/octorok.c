#include "octorok.h"

Entity *createOctorok(int spriteIndex){
    Entity *e = malloc(sizeof(Entity));
    
    e->active = 1;
    
    e->action = &doOctorok;
    e->sprite = getSprite(spriteIndex);
    
//    double pixelsPerMilli;
//    double changeX, changeY;
//	int w, h;
//	int isMoving;
//	int milliPerFrame, milliPassed;
//	int numFrames;
//	Orientation orientation;
//	void (*action)(void);
//	void (*draw)(void);
//	void (*collide)(struct Entity*);
//	CollisionType type;
//	HitBox *moveHitBox;
//	HitBox *interactHitBox;
    
    return e;
}

static void doOctorok(int delta){
//    printf("I AM OCTOROK!\n");
}
