#include "interface.h"
#include "player.h"
#include "weapon.h"
#include "data_reader.h"
#include "graphics.h"
#include <stdlib.h>

#define MAX_SHIELD_SLOTS 20

static int numToDraw = 5;
static Sprite *healthIcons;
static Animation *healthIconsAnim[MAX_SHIELD_SLOTS];
static Sprite *weaponBox;

void initInterface(){
    healthIcons = readSpriteFromFile("data/sprites/00005.sprite", malloc(sizeof(Sprite)));
    weaponBox = readSpriteFromFile("data/sprites/00023.sprite", malloc(sizeof(Sprite)));
    
    Animation *healthAnim = readAnimationFromFile("data/animations/00005.animation", malloc(sizeof(Animation)));
    healthIconsAnim[0] = healthAnim;
    size_t i;
    for (i = 1; i < MAX_SHIELD_SLOTS; i++){
        healthIconsAnim[i] = shallowCopyAnimation(healthAnim);
    }
}

void termInterface(){
    free_Sprite(healthIcons);
    free_Sprite(weaponBox);
    
    size_t i;
    for (i = 1; i < MAX_SHIELD_SLOTS; i++){
        free(healthIconsAnim[i]);
    }
    free_Animation(healthIconsAnim[0]);//frees the first animation and the member pointers
}

void updateInterface(int delta){
    //number to draw is the max over the shields/health
    numToDraw = (_player.health < _player.shields) ? _player.shields : _player.health;
    
    //update the animation and set the frame number
    size_t i;
    for (i = 0; i < numToDraw; i++){
        updateAnimation(healthIconsAnim[i], delta);
        
        if (i % 2 == 0){ //left side
            if (i < _player.health && i < _player.shields){
                setAnimationLoop(healthIconsAnim[i], 2, 0);
            } else if (i < _player.health){
                setAnimationLoop(healthIconsAnim[i], 4, 0);
            } else {
                setAnimationLoop(healthIconsAnim[i], 0, 0);
            }
        } else { //right side
            if (i < _player.health && i < _player.shields){
                setAnimationLoop(healthIconsAnim[i], 3, 0);
            } else if (i < _player.health){
                setAnimationLoop(healthIconsAnim[i], 5, 0);
            } else {
                setAnimationLoop(healthIconsAnim[i], 1, 0);
            }
        }
    }
    
    //update the animations for the small weapon icons
    if (_player.equippedAId >= 0 && _player_weapons[_player.equippedAId].iconSmall != NULL){
        _player_weapons[_player.equippedAId].iconSmall->action(_player_weapons[_player.equippedAId].iconSmall, delta);
    }
    if (_player.equippedBId >= 0 && _player_weapons[_player.equippedBId].iconSmall != NULL){
        _player_weapons[_player.equippedBId].iconSmall->action(_player_weapons[_player.equippedBId].iconSmall, delta);
    }
}

void drawInterface(){
    size_t i;
    int offset;
    
    //draw the health shields
    for (i = 0; i < numToDraw; i++){
        offset = i*healthIcons->frameWidth + (i/2)*2;
        drawAnimation(healthIcons, healthIconsAnim[i], 3 + offset, 3);
    }
    
    //draw the equipped weapons and weapon boxes
    //currently the weapon boxes have a 2 border around the area where the small icon is drawn
    offset = numToDraw*healthIcons->frameWidth + (numToDraw/2)*2 + 2;
    if (_player.equippedAId >= 0 && _player_weapons[_player.equippedAId].iconSmall != NULL){
        _player_weapons[_player.equippedAId].iconSmall->draw(_player_weapons[_player.equippedAId].iconSmall, 1+offset+2, 3);
    }
    drawAnimation(weaponBox, NULL, 1+offset, 1);
    
    
    offset = (numToDraw*healthIcons->frameWidth + (numToDraw/2)*2) + weaponBox->frameWidth + 2;
    if (_player.equippedBId >= 0 && _player_weapons[_player.equippedBId].iconSmall != NULL){
        _player_weapons[_player.equippedBId].iconSmall->draw(_player_weapons[_player.equippedBId].iconSmall, 1+offset+2, 3);
    }
    drawAnimation(weaponBox, NULL, 1+offset, 1);
    
}
