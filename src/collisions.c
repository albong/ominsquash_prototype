#include "collisions.h"
#include "hitbox.h"
#include "area.h"
#include "entity.h"
#include "player.h"
#include "weapon.h"
#include "enemy.h"
#include "door.h"

#define square(x) ((x)*(x))

/////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////
static void doWallCollisions();
static void doDoorCollisions();
static void doStairCollisions();
static void doEntityInteractCollisions();
static void doWeaponCollisions();
static void enemiesCollideWithWeapon(Weapon *w);
static void doEnemyCollisions();


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void doCollisions(){
    doWallCollisions();
    doDoorCollisions();
    doStairCollisions();
    doEntityInteractCollisions();
    doWeaponCollisions();
    doEnemyCollisions();
    
    /*
    I think we need an order to check collisions in, so as to make sure things
    happen in the proper way (loosely relates to entity type).
    We check in this order:
    Room impassable collisions (walls)
    Room entity collisions (doorways, stairs, switches)
    Projectile collisions
    Weapon Collisions
    Enemy Collisions
    */
    /*
    I think we say that only entities can collide with other entities.  So the room might
    produce its own invisible entities, distinct from the overall entitiy list,
    which we check everything against.  Then we need to add a enum type to all entites, so
    that when we detect a collision we pass a pointer to both entities into a method
    collide(Entity *e1, Entity *e2), which uses their type (None, Player, Enemy, Wall, etc)
    to determine method to pass to perform the collision logic.
    */
    /*
    Divide the room into blocks of a given size.  Loop over all entities and bucket
    by blocks which they fall into.  Then check collisions between all entities in each
    bucket.  Check everything against wall collisions.
    
    Each entity should also have a pair of lists of hitboxes - one list for rectangular,
    one for circular.
    
    Entities also maybe need an enum so we know what kind of thing they are (eg player,
    enemy, environment, etc).  This way we can pass the entitiy to a collision method
    depending on what it is colliding with.  This will also save us having to calculate
    some collisions (enemies don't need to collide with other enemies?).  Alternatively,
    we can have each entity maintain a pointer to a collision function which takes in
    the thing it collided with.
    */
}

void doWallCollisions(){
    Hitbox walls = getCurrentWalls();
    int i, j, k, collCode;

    //check the player first
    CollRect temp;
    int hitFrame = _player.e.animation->currFrame;
    for (j = 0; j < _player.e.hitboxes.movement[hitFrame].numRect; j++){
        for (k = 0; k < walls.numRect; k++){
            temp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.movement[hitFrame].rects[j].x;
            temp.y = _player.e.y + _player.e.hitboxes.movement[hitFrame].rects[j].y;
            temp.w = _player.e.hitboxes.movement[hitFrame].rects[j].w;
            temp.h = _player.e.hitboxes.movement[hitFrame].rects[j].h;
            collCode = rectangleCollide(walls.rects[k], temp);
            if (collCode){
                collideWithWallX(walls.rects[k], &_player.e, temp, collCode);
            }
            
            temp.x = _player.e.x + _player.e.hitboxes.movement[hitFrame].rects[j].x;
            temp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.movement[hitFrame].rects[j].y;
            temp.w = _player.e.hitboxes.movement[hitFrame].rects[j].w;
            temp.h = _player.e.hitboxes.movement[hitFrame].rects[j].h;
            collCode = rectangleCollide(walls.rects[k], temp);
            if (collCode){
                collideWithWallY(walls.rects[k], &_player.e, temp, collCode);
            }
        }
    }
    
    //now check all entitites
//    Entity **entityList = getRoomEntityList();
//    for (i = 0; i < getNumRoomEntities(); i++){
//        if (!entityList[i]->active || !entityList[i]->hasMoveHitbox){
//            continue;
//        }
//        hitFrame = entityList[i]->animation->currFrame;
//        for (j = 0; j < entityList[i]->moveHitbox[hitFrame].numRect; j++){
//            for (k = 0; k < walls.numRect; k++){
//                temp.x = entityList[i]->x + entityList[i]->changeX + entityList[i]->moveHitbox[hitFrame].rects[j].x;
//                temp.y = entityList[i]->y + entityList[i]->moveHitbox[hitFrame].rects[j].y;
//                temp.w = entityList[i]->moveHitbox[hitFrame].rects[j].w;
//                temp.h = entityList[i]->moveHitbox[hitFrame].rects[j].h;
//                collCode = rectangleCollide(walls.rects[k], temp);
//                if (collCode){
//                    collideWithWallX(walls.rects[k], entityList[i], temp, collCode);
//                }
//                
//                temp.x = entityList[i]->x + entityList[i]->moveHitbox[hitFrame].rects[j].x;
//                temp.y = entityList[i]->y + entityList[i]->changeY + entityList[i]->moveHitbox[hitFrame].rects[j].y;
//                temp.w = entityList[i]->moveHitbox[hitFrame].rects[j].w;
//                temp.h = entityList[i]->moveHitbox[hitFrame].rects[j].h;
//                collCode = rectangleCollide(walls.rects[k], temp);
//                if (collCode){
//                    collideWithWallY(walls.rects[k], entityList[i], temp, collCode);
//                }
//            }
//        }
//    }
}

void doDoorCollisions(){
    if (checkAreaChangeState() != DEFAULT){
        return;
    }
    
    size_t numDoors = getNumRoomDoors();
    Door **doorList = getRoomDoorList();
    CollRect temp, playerTemp;
    int i, j, k, collCode;
    
    //player first
    int hitFrame = _player.e.animation->currFrame;
    int doorHitFrame;
    for (i = 0; i < numDoors; i++){
        doorHitFrame = doorList[i]->e.animation->currFrame;
        for (j = 0; j < doorList[i]->e.hitboxes.interact[doorHitFrame].numRect; j++){
            temp.x = doorList[i]->e.x + /*doorList[i]->e.changeX +*/ doorList[i]->e.hitboxes.interact[doorHitFrame].rects[j].x;
            temp.y = doorList[i]->e.y + /*doorList[i]->e.changeY +*/ doorList[i]->e.hitboxes.interact[doorHitFrame].rects[j].y;
            temp.w = doorList[i]->e.hitboxes.interact[doorHitFrame].rects[j].w;
            temp.h = doorList[i]->e.hitboxes.interact[doorHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.interact->numRect; k++){
                playerTemp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.interact[hitFrame].rects[k].x;
                playerTemp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.interact[hitFrame].rects[k].y;
                playerTemp.w = _player.e.hitboxes.interact[hitFrame].rects[k].w;
                playerTemp.h = _player.e.hitboxes.interact[hitFrame].rects[k].h;

                collCode = rectangleCollide(playerTemp, temp);
                if (collCode && !doorList[i]->isLocked && !doorList[i]->isOpen){
                    //set door as open
                    setDoorOpening(doorList[i], 1);
                    break;
                } else if (!collCode && doorList[i]->isOpen){
                    //set door as closed
                    setDoorOpening(doorList[i], 0);
                    break;
                }
            }
        }
        
        //now gotta check movement hitboxes
    }
    
    //check if other stuff collides, or just player?
}

void doStairCollisions(){
    if (checkAreaChangeState() != DEFAULT){
        return;
    }

    CollRect temp, playerTemp;
    size_t i, j, k; 
    int collCode;
    
    size_t numStairs = getNumRoomStairs();
    Stair **stairList = getRoomStairList();
    
    int hitFrame = _player.e.animation->currFrame;
    int entityHitFrame;
    for (i = 0; i < numStairs; i++){
        if (!stairList[i]->e.active || !(stairList[i]->e.hitboxes.numMovement > 0)){
            continue;
        }
        
        entityHitFrame = stairList[i]->e.animation->currFrame;
        for (j = 0; j < stairList[i]->e.hitboxes.movement[entityHitFrame].numRect; j++){
            temp.x = stairList[i]->e.x + stairList[i]->e.changeX + stairList[i]->e.hitboxes.movement[entityHitFrame].rects[j].x;
            temp.y = stairList[i]->e.y + stairList[i]->e.changeY + stairList[i]->e.hitboxes.movement[entityHitFrame].rects[j].y;
            temp.w = stairList[i]->e.hitboxes.movement[entityHitFrame].rects[j].w;
            temp.h = stairList[i]->e.hitboxes.movement[entityHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.movement->numRect; k++){
                playerTemp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.movement[hitFrame].rects[k].x;
                playerTemp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.movement[hitFrame].rects[k].y;
                playerTemp.w = _player.e.hitboxes.movement[hitFrame].rects[k].w;
                playerTemp.h = _player.e.hitboxes.movement[hitFrame].rects[k].h;
                
                collCode = rectangleCollide(playerTemp, temp);
                if (collCode && stairList[i]->e.collidePlayer != NULL){
                    stairList[i]->e.collidePlayer((Entity *)(stairList[i]), collCode);
                }
            }
        }
    }
}

void doEntityInteractCollisions(){
    size_t numEntities = getNumRoomEntities();
    Entity **entityList = getRoomEntityList();
    CollRect temp, playerTemp;
    size_t i, j, k; 
    int collCode;

    /*
    We need to check all the enemies against the player and against all other entities - even walls?
    What if we want an enemy that absorbs other enemies, we need to check collisions between all enemies too, or do we
    make a special type of enemy that can collide with others?
    */
    
    if (!isPlayerInteractable() || checkAreaChangeState() != DEFAULT){
        return;
    }
    
    //player first
    int hitFrame = _player.e.animation->currFrame;
    int entityHitFrame;
    for (i = 0; i < numEntities; i++){
        if (!entityList[i]->active || !entityList[i]->interactable || !entityList[i]->hitboxes.numInteract > 0){
            continue;
        }
        
        entityHitFrame = entityList[i]->animation->currFrame;
        for (j = 0; j < entityList[i]->hitboxes.interact[entityHitFrame].numRect; j++){
            temp.x = entityList[i]->x + entityList[i]->changeX + entityList[i]->hitboxes.interact[entityHitFrame].rects[j].x;
            temp.y = entityList[i]->y + entityList[i]->changeY + entityList[i]->hitboxes.interact[entityHitFrame].rects[j].y;
            temp.w = entityList[i]->hitboxes.interact[entityHitFrame].rects[j].w;
            temp.h = entityList[i]->hitboxes.interact[entityHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.interact->numRect; k++){
                playerTemp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.interact[hitFrame].rects[k].x;
                playerTemp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.interact[hitFrame].rects[k].y;
                playerTemp.w = _player.e.hitboxes.interact[hitFrame].rects[k].w;
                playerTemp.h = _player.e.hitboxes.interact[hitFrame].rects[k].h;
                
                collCode = rectangleCollide(playerTemp, temp);
                if (collCode && entityList[i]->interact != NULL){
                    entityList[i]->interact(entityList[i]);
                }
            }
        }
    }
    
    //other entites?
}

void doWeaponCollisions(){
    /********************************************************
    Could we optimize this into one loop, checking both things?
    Player will (likely) only ever have 2 weapons, so that might make sense...
    ********************************************************/
    if (_player.equippedAId != -1){
        if (_player_weapons[_player.equippedAId].e.active && _player_weapons[_player.equippedAId].e.hitboxes.numInteract > 0){
            enemiesCollideWithWeapon(_player_weapons + _player.equippedAId);
        }
    }
    if (_player.equippedBId != -1){
       if (_player_weapons[_player.equippedBId].e.active && _player_weapons[_player.equippedBId].e.hitboxes.numInteract > 0){
            enemiesCollideWithWeapon(_player_weapons + _player.equippedBId);
        }
    }
    
    //enemy weapons/projectiles will go here
}

void enemiesCollideWithWeapon(Weapon *w){
    int numEnemies = getNumRoomEnemies();
    Enemy **enemyList = getRoomEnemyList();
    CollRect temp, weaponTemp;
    int i, j, k, collCode;

    /*
    We need to check all the enemies against the player and against all other entities - even walls?
    What if we want an enemy that absorbs other enemies, we need to check collisions between all enemies too, or do we
    make a special type of enemy that can collide with others?
    */
    
    //player first
    int enemyHitFrame;
    for (i = 0; i < numEnemies; i++){
        if (!enemyList[i]->e.active){
            continue;
        }
        
        enemyHitFrame = enemyList[i]->e.animation->currFrame;
        for (j = 0; j < enemyList[i]->e.hitboxes.interact->numRect; j++){
            temp.x = enemyList[i]->e.x + enemyList[i]->e.changeX + enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].x;
            temp.y = enemyList[i]->e.y + enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].y;
            temp.w = enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].w;
            temp.h = enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.interact->numRect; k++){
                weaponTemp.x = w->e.x + w->e.changeX + w->e.hitboxes.interact[w->e.animation->currFrame].rects[k].x;
                weaponTemp.y = w->e.y + w->e.changeY + w->e.hitboxes.interact[w->e.animation->currFrame].rects[k].y;
                weaponTemp.w = w->e.hitboxes.interact[w->e.animation->currFrame].rects[k].w;
                weaponTemp.h = w->e.hitboxes.interact[w->e.animation->currFrame].rects[k].h;
                
                collCode = rectangleCollide(weaponTemp, temp);
                if (collCode){
                    w->collide(w, enemyList[i], collCode, enemyList[i]->e.type);
                }
            }
        }
    }
}

void doEnemyCollisions(){
    size_t numEnemies = getNumRoomEnemies();
    Enemy **enemyList = getRoomEnemyList();
    CollRect temp, playerTemp;
    size_t i, j, k; 
    int collCode;

    /*
    We need to check all the enemies against the player and against all other entities - even walls?
    What if we want an enemy that absorbs other enemies, we need to check collisions between all enemies too, or do we
    make a special type of enemy that can collide with others?
    */
    
    //player first
    int hitFrame = _player.e.animation->currFrame;
    int enemyHitFrame;
    for (i = 0; i < numEnemies; i++){
        if (!enemyList[i]->e.active || enemyList[i]->health <= 0){
            continue;
        }
        
        enemyHitFrame = enemyList[i]->e.animation->currFrame;
        for (j = 0; j < enemyList[i]->e.hitboxes.interact->numRect; j++){
            temp.x = enemyList[i]->e.x + enemyList[i]->e.changeX + enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].x;
            temp.y = enemyList[i]->e.y + enemyList[i]->e.changeY + enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].y;
            temp.w = enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].w;
            temp.h = enemyList[i]->e.hitboxes.interact[enemyHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.interact->numRect; k++){
                playerTemp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.interact[hitFrame].rects[k].x;
                playerTemp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.interact[hitFrame].rects[k].y;
                playerTemp.w = _player.e.hitboxes.interact[hitFrame].rects[k].w;
                playerTemp.h = _player.e.hitboxes.interact[hitFrame].rects[k].h;
                
                collCode = rectangleCollide(playerTemp, temp);
                if (collCode && enemyList[i]->collidePlayer != NULL){
                    enemyList[i]->e.collidePlayer((Entity *) (enemyList[i]), collCode);
                }
            }
        }
    }
    
    //other entites?
}

int checkPlayerCollideEntitiesMovement(Entity **entityList, size_t numEntities){
    if (entityList == NULL || numEntities <= 0){
        return -1;
    }

    CollRect temp, playerTemp;
    size_t i, j, k; 
    int collCode;
    
    //player first
    int hitFrame = _player.e.animation->currFrame;
    int entityHitFrame;
    for (i = 0; i < numEntities; i++){
        if (!entityList[i]->active || !(entityList[i]->hitboxes.numMovement > 0)){
            continue;
        }
        
        entityHitFrame = entityList[i]->animation->currFrame;
        for (j = 0; j < entityList[i]->hitboxes.movement[entityHitFrame].numRect; j++){
            temp.x = entityList[i]->x + entityList[i]->changeX + entityList[i]->hitboxes.movement[entityHitFrame].rects[j].x;
            temp.y = entityList[i]->y + entityList[i]->changeY + entityList[i]->hitboxes.movement[entityHitFrame].rects[j].y;
            temp.w = entityList[i]->hitboxes.movement[entityHitFrame].rects[j].w;
            temp.h = entityList[i]->hitboxes.movement[entityHitFrame].rects[j].h;
                
            for (k = 0; k < _player.e.hitboxes.movement->numRect; k++){
                playerTemp.x = _player.e.x + _player.e.changeX + _player.e.hitboxes.movement[hitFrame].rects[k].x;
                playerTemp.y = _player.e.y + _player.e.changeY + _player.e.hitboxes.movement[hitFrame].rects[k].y;
                playerTemp.w = _player.e.hitboxes.movement[hitFrame].rects[k].w;
                playerTemp.h = _player.e.hitboxes.movement[hitFrame].rects[k].h;
                
                collCode = rectangleCollide(playerTemp, temp);
                if (collCode){
                    return i;
                }
            }
        }
    }
    
    //if the player didn't hit anything
    return -1;
}


/////////////////////////////////////////////////
// Collisions
/////////////////////////////////////////////////
int rectangleCollide(CollRect r1, CollRect r2){
    ///NOTE: sometimes you can get weird numbers returned, negatives and such; I think this occurs when rectangles are on top of each other.
    
    //1 = left, 2, = right, 3 = up, 4 = down - these are for which side of r1 is hit by r2
    int result = 0;
    
    //fast check before we check all sides
    if (r1.x+r1.w <= r2.x || r2.x+r2.w <= r1.x || r1.y+r1.h <= r2.y || r2.y+r2.h <= r1.y){
        return 0;
    }
    
    //if we have a collision, then determine the side
    if (r2.x <= r1.x && r1.x <= r2.x+r2.w){
        result |= 1;
    }
    if (r2.x <= r1.x+r1.w && r1.x+r1.w <= r2.x+r2.w){
        result |= 2;
    }
    if (r2.y <= r1.y && r1.y <= r2.y+r2.h){
        result |= 4;
    }
    if (r2.y <= r1.y+r1.h && r1.y+r1.h <= r2.y+r2.h){
        result |= 8;
    }
    if (!result){
        result = -1;
    }

    return result;
}

int circleCollide(CollCircle c1, CollCircle c2){
    if (square(c2.cx-c1.cx) + square(c2.cy-c1.cy) < square(c1.r+c2.r)){
        return 1;
    } else {
        return 0;
    }
}

int rectangleCircleCollide(CollRect r, CollCircle c){
    //check if edge of rectangle is inside of circle
    //then check if a corner of rectangle is inside of circle
    if ((r.x < c.cx && c.cx < r.x+r.w && square(c.cy-r.y) < square(c.r)) ||
            (r.x < c.cx && c.cx < r.x+r.w && square(c.cy-r.y-r.h) < square(c.r)) ||
            (r.y < c.cy && c.cy < r.y+r.h && square(c.cx-r.x) < square(c.r)) ||
            (r.y < c.cy && c.cy < r.y+r.h && square(c.cx-r.x-r.w) < square(c.r)) ||
            (square(r.x-c.cx) + square(r.y-c.cy) < square(c.r)) ||
            (square(r.x-c.cx) + square(r.y+r.h-c.cy) < square(c.r)) ||
            (square(r.x+r.w-c.cx) + square(r.y-c.cy) < square(c.r)) ||
            (square(r.x+r.w-c.cx) + square(r.y+r.h-c.cy) < square(c.r))){
        return 1;
    } else {
        return 0;
    }
}

