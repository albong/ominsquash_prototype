#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "entity.h"
#include "sound.h"
#include "text.h"

// Upon reflection, it seems like the better approach is to keep the idea of shots,
// but reduce them to the level of individual entities.  When the logic for the cutscene
// runs, it increments the counter tracking how long the cutscene has been playing.
// But when the textbox/prompt is pushed onto the frame stack, the cutscene will
// continue to be drawn (including animations if loops are correctly set), while
// the logic won't run, and the counter won't increment.

// This prevents us from having branching cutscenes, but the solution is to have a
// cutscene be composed of Scenes, with each scene composed of lots of shots (which
// more appropriately ought be called linear tweens probably).  The editor then will
// have to do more work to assure that the start of each branching scene matches the end
// of the prior scene, but eh.

//PIZZA - need to add stuff for when to draw/not draw the current room in the background

//PIZZA - as far 

typedef struct LinearTween {
    int draw;
    double startX;
    double startY;
    int isMoving;
    double endX;
    double endY;
    int animationLoop;
    int moveDuration;
} LinearTween;

typedef struct Cutscene {
    int numPlayerTweens;
    LinearTween **playerTweens;
    
    int numEntities;
    Entity **entities;
    int *numEntityTweens;
    LinearTween ***entityTweens;
    
    int numSounds;
    Sound **sounds;
    int *timeToPlay; //may not play at exactly at that time
    int *numRepeats;
    
    int numMusic;
    Music **music;
    int *musicStartTime;
    int *musicEndTime;
    int *fadeInDuration;
    int *fadeOutDuration;
    
    int numText;
    Text **text;
    int *showTextAtTime;
} Cutscene;

// Loading/Unloading
LinearTween *init_LinearTween(LinearTween *self);
Cutscene *init_Cutscene(Cutscene *self);
void free_LinearTween(LinearTween *self);
void free_Cutscene(Cutscene *self);

//Logic
int doCutscene(unsigned delta);

//Drawing
void drawCutscene();

#endif