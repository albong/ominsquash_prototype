#include "collisions.h"
#include "area.h"
#include "entity.h"
#include "player.h"

#define square(x) (x*x)

void doCollisions(){
    doWallCollisions();
    doEnemyCollisions();
    /*
    I think we need an order to check collisions in, so as to make sure things
    happen in the proper way (loosely relates to entity type).
    We check in this order:
    Room impassable collisions (walls)
    Room entity collisions (doorways, stairs, switches)
    Projectile collisions
    Enemy Collisions
    */
    /*
    I think we say that only entities can collide with other entities.  So the room might
    produce its own invisible entities, distinct from the overall entitiy list,
    which we check everything against.  Then we need to add a enum type to all entites, so
    that when we detect a collision we pass a pointer to both entities into a method
    collide(Entity *e1, Entity *e2), which uses their type (None, Player, Enemy, Wall, etc)
    to determine what method to pass to perform the collision logic.
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

static void doWallCollisions(){
    HitBox walls = getCurrentWalls();
    int i, j, k, collCode;

    //check the player first
    CollRect temp;
    for (j = 0; j < _player.moveHitBox->numRect; j++){
        for (k = 0; k < walls.numRect; k++){
            temp.x = _player.x + _player.changeX + _player.moveHitBox->rects[j].x;
            temp.y = _player.y + _player.moveHitBox->rects[j].y;
            temp.w = _player.moveHitBox->rects[j].w;
            temp.h = _player.moveHitBox->rects[j].h;
            collCode = rectangleCollide(walls.rects[k], temp);
            if (collCode){
                collideWithWallX(walls.rects[k], &_player, temp, collCode);
            }
            
            temp.x = _player.x + _player.moveHitBox->rects[j].x;
            temp.y = _player.y + _player.changeY + _player.moveHitBox->rects[j].y;
            temp.w = _player.moveHitBox->rects[j].w;
            temp.h = _player.moveHitBox->rects[j].h;
            collCode = rectangleCollide(walls.rects[k], temp);
            if (collCode){
                collideWithWallY(walls.rects[k], &_player, temp, collCode);
            }
        }
    }
    
    //now check all entitites
//    for (i = 0; i < MAX_ENTITIES && entity[i].active; i++){
//        for (j = 0; j < entity[i].moveHitBox->numRect; j++){
//            for (k = 0; k < walls.numRect; k++){
//                temp.x = entity[i].x + entity[i].changeX + entity[i].moveHitBox->rects[j].x;
//                temp.y = entity[i].y + entity[i].moveHitBox->rects[j].y;
//                temp.w = entity[i].moveHitBox->rects[j].w;
//                temp.h = entity[i].moveHitBox->rects[j].h;
//                collCode = rectangleCollide(walls.rects[k], temp);
//                if (collCode){
//                    collideWithWallX(walls.rects[k], &entity[i], temp, collCode);
//                }
//                
//                temp.x = entity[i].x + entity[i].moveHitBox->rects[j].x;
//                temp.y = entity[i].y + entity[i].changeY + entity[i].moveHitBox->rects[j].y;
//                temp.w = entity[i].moveHitBox->rects[j].w;
//                temp.h = entity[i].moveHitBox->rects[j].h;
//                collCode = rectangleCollide(walls.rects[k], temp);
//                if (collCode){
//                    collideWithWallY(walls.rects[k], &entity[i], temp, collCode);
//                }
//            }
//        }
//    }
}

static void doEnemyCollisions(){
    int numEntities = getNumRoomEntities();
    Entity **entityList = getRoomEntityList();
    CollRect temp, playerTemp;
    int i, j, k, collCode;

    /*
    We need to check all the enemies against the player and against all other entities - even walls?
    What if we want an enemy that absorbs other enemies, we need to check collisions between all enemies too, or do we
    make a special type of enemy that can collide with others?
    */
    
    //player first
    for (i = 0; i < numEntities && entityList[i]->active && (entityList[i]->type == ENEMY || entityList[i]->type == ENEMY_COLL); i++){
        for (j = 0; j < entityList[i]->interactHitBox->numRect; j++){
            temp.x = entityList[i]->x + entityList[i]->changeX + entityList[i]->interactHitBox->rects[j].x;
            temp.y = entityList[i]->y + entityList[i]->interactHitBox->rects[j].y;
            temp.w = entityList[i]->interactHitBox->rects[j].w;
            temp.h = entityList[i]->interactHitBox->rects[j].h;
                
            for (k = 0; k < _player.interactHitBox->numRect; k++){
                playerTemp.x = _player.x + _player.changeX + _player.interactHitBox->rects[k].x;
                playerTemp.y = _player.y + _player.changeY + _player.interactHitBox->rects[k].y;
                playerTemp.w = _player.interactHitBox->rects[k].w;
                playerTemp.h = _player.interactHitBox->rects[k].h;
                
                collCode = rectangleCollide(playerTemp, temp);
                if (collCode){
                    playerCollideWithEnemy(entityList[i], collCode);
                }
            }
        }
    }
    
    //other entites?
}

static int rectangleCollide(CollRect r1, CollRect r2){
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

static int circleCollide(CollCircle c1, CollCircle c2){
    if (square(c2.cx-c1.cx) + square(c2.cy-c1.cy) < square(c1.r+c2.r)){
        return 1;
    } else {
        return 0;
    }
}

static int rectangleCircleCollide(CollRect r, CollCircle c){
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

