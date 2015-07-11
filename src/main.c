#include "main.h"
#include "SDL/SDL.h"
#include "input.h"
#include "player.h"
#include "collisions.h"
#include "area.h"
#include "weapon.h"

#include "../debug/hitbox_drawer.h"

int main(int argc, char *argv[]){
//    unsigned int frameLimit = SDL_GetTicks() + 16;
    unsigned currMilliseconds = 0;
    unsigned prevMilliseconds = 0;
    int delta;
    int go = 1;
    
    //start the reactor
    initSDL();
	freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
    
    //when the program exits, clean everything up
    atexit(stopSDL);
    
    loadAnimatedSprite("gfx/linksprite.png", 15);
    
    initWeaponLists(); //before the area and player
    
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

        //do logic, calculate next positions
        doPlayer(delta);
        doRoom(delta);
        
        //do any collisions
        doCollisions();

        //update positions
        movePlayer();
        moveRoomEntities();
        moveRoomEnemies();
        
        //draw
        clearScreen();
        drawCurrentRoom();
        drawPlayer();
//        drawHitBoxes(DRAW_MOVE_HITBOX, DRAW_INTERACT_HITBOX);
        bufferToScreen();
        
        //sleep?  I don't recall why
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
