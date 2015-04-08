#include "collisions.h"
#include "area.h"
#include "entity.h"
#include "player.h"

#define square(x) (x*x)

void doCollisions(){
    doWallCollisions();
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
    int collCode2;
    //check the player first
    
    
    //
    //
    // PLAYER'S C COLLISION RECTANGLE HAS RELATIVE POSITION:
    // NEED TO ADD PLAYER'S POSITION TO X & Y TO GET ACTUAL RECTANGLE
    // RECOMMEND MAKING A TEMP CollRect TO STORE THIS DATA IN
    //
    //
    CollRect temp;
    for (j = 0; j < player.moveHitBox->numRect; j++){
        for (k = 0; k < walls.numRect; k++){
            temp.x = player.x + player.changeX + player.moveHitBox->rects[j].x;
            temp.y = player.y + player.moveHitBox->rects[j].y;
            temp.w = player.moveHitBox->rects[j].w;
            temp.h = player.moveHitBox->rects[j].h;
//            collCode = rectangleCollide(walls.rects[k], player.moveHitBox->rects[j]);
            collCode = rectangleCollide(walls.rects[k], temp);
            
            temp.x = player.x + player.moveHitBox->rects[j].x;
            temp.y = player.y + player.changeY + player.moveHitBox->rects[j].y;
            temp.w = player.moveHitBox->rects[j].w;
            temp.h = player.moveHitBox->rects[j].h;
//            collCode = rectangleCollide(walls.rects[k], player.moveHitBox->rects[j]);
            collCode2 = rectangleCollide(walls.rects[k], temp);
            if (collCode || collCode2){
                printf("------\n");
                printf("%f, %f - %f, %f\n", player.y, player.y+player.h, player.y+player.changeY, player.y+player.h+player.changeY);
                if (collCode){
                    collideWithWall(walls.rects[k], &player, temp, collCode, 1);
                }
                if (collCode2){
                    collideWithWall(walls.rects[k], &player, temp, collCode, 0);
                }
                printf("%d,%d : %d - %d\n", j, k, collCode, collCode2);
                printf("%f, %f - %f, %f\n", player.y, player.y+player.h, player.y+player.changeY, player.y+player.h+player.changeY);
                printf("-------\n");
            }
        }
    }
    
    //now check all entitites
    for (i = 0; i < MAX_ENTITIES && entity[i].active; i++){
        for (j = 0; j < entity[i].moveHitBox->numRect; j++){
            for (k = 0; k < walls.numRect; k++){
                collCode = rectangleCollide(walls.rects[k], entity[i].moveHitBox->rects[j]);
                if (collCode > 0){
                    collideWithWall(walls.rects[k], &entity[i], temp, collCode, 0);
                    
                }
            }
        }
    }
}

/**
 ** THINK THIS HAS PROBLEMS - what if one rectangle is inside the other?
 **/
static int rectangleCollide(CollRect r1, CollRect r2){
    //1 = left, 2, = right, 3 = up, 4 = down - these are for which side of r1 is hit by r2
    int result = 0;
    
//    //fast check before we check all sides
//    if (r1.x+r1.w <= r2.x || r2.x+r2.w <= r1.x 
//        || r1.y+r1.h <= r2.y || r2.y+r2.h <= r1.y){
//        return 0;
//    }
    if (r1.x+r1.w <= r2.x || r2.x+r2.w <= r1.x 
        || r1.y+r1.h <= r2.y || r2.y+r2.h <= r1.y){
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
//    printf("%f, %f - %f, %f\n", r1.x, r1.x+r1.w, r2.x, r2.x+r2.w);
//    printf("%d\n", result);

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

