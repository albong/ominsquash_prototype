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

void drawHitboxes(int drawMove, int drawInteract){
    if (drawMove){
        drawMoveHitboxes();
    }
    if (drawInteract){
        drawInteractHitboxes();
    }
}

void drawMoveHitboxes(){
    int i, j, k;
    int x, y, w, h;
    
    //draw the player's movement box
    if (_player.e.hitboxes.numMovement > 0){
        for (i = 0; i < _player.e.hitboxes.movement[_player.e.animation->currFrame].numRect; i++){
            x = _player.e.x + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[i].x;
            y = _player.e.y + _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[i].y;
            w = _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[i].w;
            h = _player.e.hitboxes.movement[_player.e.animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the player's weapons
    Weapon *wep;
    if (_player.equippedAInd != -1){
        wep = _player_weapons.weapons[_player.equippedAInd];
        if (wep->e.hitboxes.numMovement > 0  && wep->e.active){
            for (i = 0; i < wep->e.hitboxes.movement[wep->e.animation->currFrame].numRect; i++){
                x = wep->e.x + wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].x;
                y = wep->e.y + wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].y;
                w = wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].w;
                h = wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].h;
                drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
            }
        }
    }
    if (_player.equippedBInd != -1){
        wep = _player_weapons.weapons[_player.equippedBInd];
        if (wep->e.hitboxes.numMovement > 0  && wep->e.active){
            for (i = 0; i < wep->e.hitboxes.movement[wep->e.animation->currFrame].numRect; i++){
                x = wep->e.x + wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].x;
                y = wep->e.y + wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].y;
                w = wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].w;
                h = wep->e.hitboxes.movement[wep->e.animation->currFrame].rects[i].h;
                drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
            }
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    Entity *temp;
    int hitFrame;
    for (j = 0; j < getNumRoomEntities(); j++){
        if (!entityList[j]->active || !entityList[j]->hitboxes.numMovement > 0){
            continue;
        }
        
        temp = entityList[j];
        hitFrame = entityList[j]->animation->currFrame;
        for (i = 0; i < entityList[j]->hitboxes.movement[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.movement[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.movement[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.movement[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.movement[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (!enemyList[j]->e.active || !enemyList[j]->e.hitboxes.numMovement > 0){
            continue;
        }
        
        temp = (Entity *)enemyList[j];
        hitFrame = temp->animation->currFrame;
        for (i = 0; i < temp->hitboxes.movement[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.movement[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.movement[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.movement[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.movement[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (!doorList[j]->e.active || !doorList[j]->e.hitboxes.numMovement > 0){
            continue;
        }
        
        temp = (Entity *)doorList[j];
        hitFrame = temp->animation->currFrame;
        for (i = 0; i < temp->hitboxes.movement[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.movement[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.movement[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.movement[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.movement[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 255, 0, 0);
        }
    }
}

void drawInteractHitboxes(){
    int i, j, k;
    int x, y, w, h;
    
    //draw the player's interact box
    if (_player.e.hitboxes.numInteract > 0){
        for (i = 0; i < _player.e.hitboxes.interact[_player.e.animation->currFrame].numRect; i++){
            x = _player.e.x + _player.e.hitboxes.interact[_player.e.animation->currFrame].rects[i].x;
            y = _player.e.y + _player.e.hitboxes.interact[_player.e.animation->currFrame].rects[i].y;
            w = _player.e.hitboxes.interact[_player.e.animation->currFrame].rects[i].w;
            h = _player.e.hitboxes.interact[_player.e.animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the player's weapons
    Weapon *wep;
    if (_player.equippedAInd != -1){
        wep = _player_weapons.weapons[_player.equippedAInd];
        if (wep->e.hitboxes.numInteract > 0 && wep->e.active){
            for (i = 0; i < wep->e.hitboxes.interact[wep->e.animation->currFrame].numRect; i++){
                x = wep->e.x + wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].x;
                y = wep->e.y + wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].y;
                w = wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].w;
                h = wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].h;
                drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
            }
        }
    }
    if (_player.equippedBInd != -1){
        wep = _player_weapons.weapons[_player.equippedBInd];
        if (wep->e.hitboxes.numInteract > 0 && wep->e.active){
            for (i = 0; i < wep->e.hitboxes.interact[wep->e.animation->currFrame].numRect; i++){
                x = wep->e.x + wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].x;
                y = wep->e.y + wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].y;
                w = wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].w;
                h = wep->e.hitboxes.interact[wep->e.animation->currFrame].rects[i].h;
                drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
            }
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    Entity *temp;
    int hitFrame;
    for (j = 0; j < getNumRoomEntities(); j++){
        if (!entityList[j]->active || !entityList[j]->hitboxes.numInteract > 0){
            continue;
        }
        
        temp = entityList[j];
        hitFrame = entityList[j]->animation->currFrame;
        for (i = 0; i < entityList[j]->hitboxes.interact[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.interact[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.interact[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.interact[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.interact[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (!enemyList[j]->e.active || !enemyList[j]->e.hitboxes.numInteract > 0){
            continue;
        }
        
        temp = (Entity *)enemyList[j];
        hitFrame = temp->animation->currFrame;
        for (i = 0; i < temp->hitboxes.interact[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.interact[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.interact[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.interact[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.interact[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (!doorList[j]->e.active || !doorList[j]->e.hitboxes.numInteract > 0){
            continue;
        }
        
        temp = (Entity *)doorList[j];
        hitFrame = temp->animation->currFrame;
        for (i = 0; i < temp->hitboxes.interact[hitFrame].numRect; i++){
            x = temp->x + temp->hitboxes.interact[temp->animation->currFrame].rects[i].x;
            y = temp->y + temp->hitboxes.interact[temp->animation->currFrame].rects[i].y;
            w = temp->hitboxes.interact[temp->animation->currFrame].rects[i].w;
            h = temp->hitboxes.interact[temp->animation->currFrame].rects[i].h;
            drawUnfilledRect_T(x, y, w, h, 0, 0, 255);
        }
    }
}
