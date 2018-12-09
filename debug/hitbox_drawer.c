#include "hitbox_drawer.h"
#include "../src/graphics.h"
#include "../src/entity.h"
#include "../src/player.h"
#include "../src/area.h"
#include "../src/weapon.h"
#include "../src/enemy.h"
#include "../src/door.h"
#include <stdio.h>

static void drawMoveHitboxes();
static void drawInteractHitboxes();
static void drawMoveHitboxesEntity(Entity *e);
static void drawInteractHitboxesEntity(Entity *e);

void drawHitboxes(int drawMove, int drawInteract){
    if (drawMove){
        drawMoveHitboxes();
    }
    if (drawInteract){
        drawInteractHitboxes();
    }
}

void drawMoveHitboxes(){
    int j;
    
    //draw the player's movement box
    if (_player.e.active && _player.e.hitboxes.numMovement > 0){
        drawMoveHitboxesEntity((Entity *)(&_player));
    }
    
    //draw the player's weapons
    Weapon *wep; //lordy why
    if (_player.equippedAId != -1){
        wep = _player_weapons + _player.equippedAId;
        if (wep->e.hitboxes.numMovement > 0  && wep->e.active){
            drawMoveHitboxesEntity((Entity *)wep);
        }
    }
    if (_player.equippedBId != -1){
        wep = _player_weapons + _player.equippedBId;
        if (wep->e.hitboxes.numMovement > 0  && wep->e.active){
            drawMoveHitboxesEntity((Entity *)wep);
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    for (j = 0; j < getNumRoomEntities(); j++){
        if (entityList[j]->active && entityList[j]->hitboxes.numMovement > 0){
            drawMoveHitboxesEntity(entityList[j]);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (enemyList[j]->e.active && enemyList[j]->e.hitboxes.numMovement > 0){
            drawMoveHitboxesEntity((Entity *)(enemyList[j]));
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (doorList[j]->e.active && doorList[j]->e.hitboxes.numMovement > 0){
            drawMoveHitboxesEntity((Entity *)(doorList[j]));
        }
    }
    
    //draw the room's stairs
    Stair **stairList = getRoomStairList();
    for (j = 0; j < getNumRoomStairs(); j++){
        if (stairList[j]->e.active && stairList[j]->e.hitboxes.numMovement > 0){
            drawMoveHitboxesEntity((Entity *)(stairList[j]));
        }
    }
}

void drawInteractHitboxes(){
    int j;
    
    //draw the player's movement box
    if (_player.e.active && _player.e.hitboxes.numInteract > 0){
        drawInteractHitboxesEntity((Entity *)(&_player));
    }
    
    //draw the player's weapons
    Weapon *wep;
    if (_player.equippedAId != -1){
        wep = _player_weapons + _player.equippedAId;
        if (wep->e.hitboxes.numInteract > 0  && wep->e.active){
            drawInteractHitboxesEntity((Entity *)wep);
        }
    }
    if (_player.equippedBId != -1){
        wep = _player_weapons + _player.equippedBId;
        if (wep->e.hitboxes.numInteract > 0  && wep->e.active){
            drawInteractHitboxesEntity((Entity *)wep);
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    for (j = 0; j < getNumRoomEntities(); j++){
        if (entityList[j]->active && entityList[j]->hitboxes.numInteract > 0){
            drawInteractHitboxesEntity(entityList[j]);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (enemyList[j]->e.active && enemyList[j]->e.hitboxes.numInteract > 0){
            drawInteractHitboxesEntity((Entity *)(enemyList[j]));
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (doorList[j]->e.active && doorList[j]->e.hitboxes.numInteract > 0){
            drawInteractHitboxesEntity((Entity *)(doorList[j]));
        }
    }
    
    //draw the room's stairs
    Stair **stairList = getRoomStairList();
    for (j = 0; j < getNumRoomStairs(); j++){
        if (stairList[j]->e.active && stairList[j]->e.hitboxes.numInteract > 0){
            drawInteractHitboxesEntity((Entity *)(stairList[j]));
        }
    }
}

static void drawMoveHitboxesEntity(Entity *self){
    int i;
    int x, y, w, h;
    
    int hitFrame = self->animation->currFrame;
    for (i = 0; i < self->hitboxes.movement[hitFrame].numRect; i++){
        x = self->x + self->hitboxes.movement[self->animation->currFrame].rects[i].x;
        y = self->y + self->hitboxes.movement[self->animation->currFrame].rects[i].y;
        w = self->hitboxes.movement[self->animation->currFrame].rects[i].w;
        h = self->hitboxes.movement[self->animation->currFrame].rects[i].h;
        drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
    }
}

static void drawInteractHitboxesEntity(Entity *self){
    int i;
    int x, y, w, h;
    
    int hitFrame = self->animation->currFrame;
    for (i = 0; i < self->hitboxes.interact[hitFrame].numRect; i++){
        x = self->x + self->hitboxes.interact[self->animation->currFrame].rects[i].x;
        y = self->y + self->hitboxes.interact[self->animation->currFrame].rects[i].y;
        w = self->hitboxes.interact[self->animation->currFrame].rects[i].w;
        h = self->hitboxes.interact[self->animation->currFrame].rects[i].h;
        drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
    }
}
