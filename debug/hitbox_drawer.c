#include "hitbox_drawer.h"
#include "../src/graphics.h"
#include "../src/entity.h"
#include "../src/player.h"
#include "../src/area.h"
#include "../src/weapon.h"
#include "../src/enemy.h"
#include "../src/door.h"

static void drawMoveHitBoxes();
static void drawInteractHitBoxes();

void drawHitBoxes(int drawMove, int drawInteract){
    if (drawMove){
        drawMoveHitBoxes();
    }
    if (drawInteract){
        drawInteractHitBoxes();
    }
}

void drawMoveHitBoxes(){
    int i, j, k;
    int x, y, w, h;
    
    //draw the player's movement box
    if (_player.e.hasMoveHitBox){
        for (i = 0; i < _player.e.moveHitBox[_player.e.currHitBox].numRect; i++){
            x = _player.e.x + _player.e.moveHitBox[_player.e.currHitBox].rects[i].x;
            y = _player.e.y + _player.e.moveHitBox[_player.e.currHitBox].rects[i].y;
            w = _player.e.moveHitBox[_player.e.currHitBox].rects[i].w;
            h =_player.e.moveHitBox[_player.e.currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the player's weapons
    Weapon *wep;
    if (_player.equippedAInd != -1){
        wep = _player_weapons.weapons[_player.equippedAInd];
        if (wep->e.hasMoveHitBox  && wep->e.active){
            for (i = 0; i < wep->e.moveHitBox[wep->e.currHitBox].numRect; i++){
                x = wep->e.x + wep->e.moveHitBox[wep->e.currHitBox].rects[i].x;
                y = wep->e.y + wep->e.moveHitBox[wep->e.currHitBox].rects[i].y;
                w = wep->e.moveHitBox[wep->e.currHitBox].rects[i].w;
                h = wep->e.moveHitBox[wep->e.currHitBox].rects[i].h;
                drawUnfilledRect(x, y, w, h, 255, 0, 0);
            }
        }
    }
    if (_player.equippedBInd != -1){
        wep = _player_weapons.weapons[_player.equippedBInd];
        if (wep->e.hasMoveHitBox  && wep->e.active){
            for (i = 0; i < wep->e.moveHitBox[wep->e.currHitBox].numRect; i++){
                x = wep->e.x + wep->e.moveHitBox[wep->e.currHitBox].rects[i].x;
                y = wep->e.y + wep->e.moveHitBox[wep->e.currHitBox].rects[i].y;
                w = wep->e.moveHitBox[wep->e.currHitBox].rects[i].w;
                h = wep->e.moveHitBox[wep->e.currHitBox].rects[i].h;
                drawUnfilledRect(x, y, w, h, 255, 0, 0);
            }
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    Entity *temp;
    int hitFrame;
    for (j = 0; j < getNumRoomEntities(); j++){
        if (!entityList[j]->active || !entityList[j]->hasMoveHitBox){
            continue;
        }
        
        temp = entityList[j];
        hitFrame = entityList[j]->currHitBox;
        for (i = 0; i < entityList[j]->moveHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->moveHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->moveHitBox[temp->currHitBox].rects[i].y;
            w = temp->moveHitBox[temp->currHitBox].rects[i].w;
            h = temp->moveHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (!enemyList[j]->e.active || !enemyList[j]->e.hasMoveHitBox){
            continue;
        }
        
        temp = (Entity *)enemyList[j];
        hitFrame = temp->currHitBox;
        for (i = 0; i < temp->moveHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->moveHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->moveHitBox[temp->currHitBox].rects[i].y;
            w = temp->moveHitBox[temp->currHitBox].rects[i].w;
            h = temp->moveHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 255, 0, 0);
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (!doorList[j]->e.active || !doorList[j]->e.hasMoveHitBox){
            continue;
        }
        
        temp = (Entity *)doorList[j];
        hitFrame = temp->currHitBox;
        for (i = 0; i < temp->moveHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->moveHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->moveHitBox[temp->currHitBox].rects[i].y;
            w = temp->moveHitBox[temp->currHitBox].rects[i].w;
            h = temp->moveHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 255, 0, 0);
        }
    }
}

void drawInteractHitBoxes(){
    int i, j, k;
    int x, y, w, h;
    
    //draw the player's interact box
    if (_player.e.hasInteractHitBox){
        for (i = 0; i < _player.e.interactHitBox[_player.e.currHitBox].numRect; i++){
            x = _player.e.x + _player.e.interactHitBox[_player.e.currHitBox].rects[i].x;
            y = _player.e.y + _player.e.interactHitBox[_player.e.currHitBox].rects[i].y;
            w = _player.e.interactHitBox[_player.e.currHitBox].rects[i].w;
            h =_player.e.interactHitBox[_player.e.currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the player's weapons
    Weapon *wep;
    if (_player.equippedAInd != -1){
        wep = _player_weapons.weapons[_player.equippedAInd];
        if (wep->e.hasInteractHitBox && wep->e.active){
            for (i = 0; i < wep->e.interactHitBox[wep->e.currHitBox].numRect; i++){
                x = wep->e.x + wep->e.interactHitBox[wep->e.currHitBox].rects[i].x;
                y = wep->e.y + wep->e.interactHitBox[wep->e.currHitBox].rects[i].y;
                w = wep->e.interactHitBox[wep->e.currHitBox].rects[i].w;
                h = wep->e.interactHitBox[wep->e.currHitBox].rects[i].h;
                drawUnfilledRect(x, y, w, h, 0, 0, 255);
            }
        }
    }
    if (_player.equippedBInd != -1){
        wep = _player_weapons.weapons[_player.equippedBInd];
        if (wep->e.hasInteractHitBox && wep->e.active){
            for (i = 0; i < wep->e.interactHitBox[wep->e.currHitBox].numRect; i++){
                x = wep->e.x + wep->e.interactHitBox[wep->e.currHitBox].rects[i].x;
                y = wep->e.y + wep->e.interactHitBox[wep->e.currHitBox].rects[i].y;
                w = wep->e.interactHitBox[wep->e.currHitBox].rects[i].w;
                h = wep->e.interactHitBox[wep->e.currHitBox].rects[i].h;
                drawUnfilledRect(x, y, w, h, 0, 0, 255);
            }
        }
    }
    
    //draw the room's entities
    Entity **entityList = getRoomEntityList();
    Entity *temp;
    int hitFrame;
    for (j = 0; j < getNumRoomEntities(); j++){
        if (!entityList[j]->active || !entityList[j]->hasInteractHitBox){
            continue;
        }
        
        temp = entityList[j];
        hitFrame = entityList[j]->currHitBox;
        for (i = 0; i < entityList[j]->interactHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->interactHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->interactHitBox[temp->currHitBox].rects[i].y;
            w = temp->interactHitBox[temp->currHitBox].rects[i].w;
            h = temp->interactHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the room's enemies
    Enemy **enemyList = getRoomEnemyList();
    for (j = 0; j < getNumRoomEnemies(); j++){
        if (!enemyList[j]->e.active || !enemyList[j]->e.hasInteractHitBox){
            continue;
        }
        
        temp = (Entity *)enemyList[j];
        hitFrame = temp->currHitBox;
        for (i = 0; i < temp->interactHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->interactHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->interactHitBox[temp->currHitBox].rects[i].y;
            w = temp->interactHitBox[temp->currHitBox].rects[i].w;
            h = temp->interactHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 0, 0, 255);
        }
    }
    
    //draw the room's doors
    Door **doorList = getRoomDoorList();
    for (j = 0; j < getNumRoomDoors(); j++){
        if (!doorList[j]->e.active || !doorList[j]->e.hasInteractHitBox){
            continue;
        }
        
        temp = (Entity *)doorList[j];
        hitFrame = temp->currHitBox;
        for (i = 0; i < temp->interactHitBox[hitFrame].numRect; i++){
            x = temp->x + temp->interactHitBox[temp->currHitBox].rects[i].x;
            y = temp->y + temp->interactHitBox[temp->currHitBox].rects[i].y;
            w = temp->interactHitBox[temp->currHitBox].rects[i].w;
            h = temp->interactHitBox[temp->currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h, 0, 0, 255);
        }
    }
}
