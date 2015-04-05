#include "main.h"
#include "SDL/SDL.h"
#include "init.h"
#include "input.h"
#include "player.h"
#include "collisions.h"
#include "area.h"

int main(int argc, char *argv[]){
//    unsigned int frameLimit = SDL_GetTicks() + 16;
    unsigned currMilliseconds = 0;
    unsigned prevMilliseconds = 0;
    int delta;
    int go = 1;
    
    //start the reactor
    init("omnisquash");
    
    //when the program exits, clean everything up
    atexit(cleanup);
    
    loadAllSprites();
    
    loadArea();
    
    //loadAllSounds();
    
    //loadAllAnimations();
    
    //set the font
    
    initPlayer();
    
    while (go){
        //calculate the time delta
        prevMilliseconds = currMilliseconds;
        currMilliseconds = SDL_GetTicks();
        delta = currMilliseconds - prevMilliseconds;
        
        getInput();
        
        doPlayer(delta);
        
        doEntities();
        
        doRoom(delta);
        
        doCollisions();
        
        movePlayer();
        moveEntities();
        
        draw();
        
        SDL_Delay(16);
    }
    
    return 0;
}
/*
We need to correctly handle frame-capping.  The best way to do this it would seem
would be to calculate
timeDelta=(time at start of current iteration)-(time at end of last iteration)
and then move accordingly.

This is all well and good for movement, but it introduces fun with collisions.
This is why there is an invulnerability period following attacks (number of collisions
at 30fps less than at 60fps), though for preventing clipping into walls/items I'm
not sure how to solve that yet.
*/

/*
Probs ought to move a ton of this junk into configuration files after we get a semi-stable
"engine" setup going on eg
player health/items/velocity, player sprites, animated sprite details (num frames, frame width), enemy sprites/sprite indices
*/
