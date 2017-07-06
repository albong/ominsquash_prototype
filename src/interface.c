#include "interface.h"
#include "player.h"
#include "data_reader.h"
#include "graphics.h"

#define MAX_SHIELD_SLOTS 20

static int numToDraw = 5;
static Sprite *healthIcons;
static SpriteAnimation *healthIconsAnim[MAX_SHIELD_SLOTS];

void initInterface(){
    healthIcons = readSpriteFromFile("data/sprites/00005.sprite", malloc(sizeof(Sprite)));
    
    SpriteAnimation *healthAnim = readSpriteAnimationFromFile("data/animations/00005.animation", malloc(sizeof(SpriteAnimation)));
    healthIconsAnim[0] = healthAnim;
    size_t i;
    for (i = 1; i < MAX_SHIELD_SLOTS; i++){
        healthIconsAnim[i] = shallowCopySpriteAnimation(healthAnim);
    }
}

void updateInterface(){
    //number to draw is the max over the shields/health
    numToDraw = (_player.health < _player.shields) ? _player.shields : _player.health;
    
    //set the frame number
    size_t i;
    for (i = 0; i < numToDraw; i++){
        if (i % 2 == 0){ //left side
            if (i < _player.health && i < _player.shields){
                healthIconsAnim[i]->currLoop = 2;
            } else if (i < _player.health){
                healthIconsAnim[i]->currLoop = 4;
            } else {
                healthIconsAnim[i]->currLoop = 0;
            }
        } else { //right side
            if (i < _player.health && i < _player.shields){
                healthIconsAnim[i]->currLoop = 3;
            } else if (i < _player.health){
                healthIconsAnim[i]->currLoop = 5;
            } else {
                healthIconsAnim[i]->currLoop = 1;
            }
        }
    }
}

void drawInterface(){
    size_t i;
    int offset;
    for (i = 0; i < numToDraw; i++){
        offset = i*healthIcons->frameWidth + (i/2)*2;
        drawAnimation(healthIcons, healthIconsAnim[i], 3 + offset, 3);
    }
}