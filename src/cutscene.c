#include "cutscene.h"
#include "logging.h"
#include "graphics.h"
#include <stdlib.h>
#include <stddef.h>

LinearTween *init_LinearTween(LinearTween *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->draw =0;
    self->startX = 0;
    self->startY = 0;
    self->isMoving = 0;
    self->endX = 0;
    self->endY = 0;
    self->animationLoop = 0;
    self->moveDuration = 0;
    
    LOG_INF("LinearTween at %p initialized", self);
    return self;
}

Cutscene *init_Cutscene(Cutscene *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->numPlayerTweens = 0;
    self->playerTweens = NULL;
    
    self->numEntities = 0;
    self->entities = NULL;
    self->numEntityTweens = 0;
    self->entityTweens = NULL;
    
    self->numSounds = 0;
    self->sounds = NULL;
    self->timeToPlay = NULL;
    self->numRepeats = NULL;
    
    self->numMusic = 0;
    self->music = NULL;
    self->musicStartTime = NULL;
    self->musicEndTime = NULL;
    self->fadeInDuration = NULL;
    self->fadeOutDuration = NULL;
    
    self->numText = 0;
    self->text = NULL;
    self->showTextAtTime = NULL;
    
    LOG_INF("Cutscene at %p initialized", self);
    return self;
}

void free_LinearTween(LinearTween *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    free(self);
    LOG_INF("LinearTween at %p freed", self);
}

void free_Cutscene(Cutscene *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    //freeing all held memory is safe, it is allocated specially for this cutscene
}

int doCutscene(unsigned delta){
    //return 1 on finished/skipped, 0 otherwise
}

void drawCutscene(){
    
}




