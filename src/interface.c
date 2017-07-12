#include "interface.h"
#include "player.h"
#include "data_reader.h"
#include "graphics.h"

#define MAX_SHIELD_SLOTS 20

static int numToDraw = 5;
static Sprite *healthIcons;
static Animation *healthIconsAnim[MAX_SHIELD_SLOTS];

void initInterface(){
    healthIcons = readSpriteFromFile("data/sprites/00005.sprite", malloc(sizeof(Sprite)));
    
    Animation *healthAnim = readAnimationFromFile("data/animations/00005.animation", malloc(sizeof(Animation)));
    healthIconsAnim[0] = healthAnim;
    size_t i;
    for (i = 1; i < MAX_SHIELD_SLOTS; i++){
        healthIconsAnim[i] = shallowCopyAnimation(healthAnim);
    }
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
}

void drawInterface(){
    size_t i;
    int offset;
    for (i = 0; i < numToDraw; i++){
        offset = i*healthIcons->frameWidth + (i/2)*2;
        drawAnimation(healthIcons, healthIconsAnim[i], 3 + offset, 3);
    }
}