#include "SDL2/SDL.h"
#include "input.h"
#include "player.h"
// #include "collisions.h"
#include "area.h"
#include "weapon.h"
#include "frames.h"
// #include "constants.h"
#include "textbox.h"
#include "inventory.h"
#include "interface.h"
#include "entity_creator.h"
#include "enemy_creator.h"
#include "title.h"
#include "load_screen.h"

#include "../debug/hitbox_drawer.h"

#include <stdio.h>

//Refer to http://gamedev.stackexchange.com/a/132835 for changes

int main(int argc, char *argv[]){
//    unsigned int frameLimit = SDL_GetTicks() + 16;
    unsigned currMilliseconds = 0;
    unsigned prevMilliseconds = 0;
    int delta;
    int go = 1;
    int i;
    
    //start the reactor
    initSDL();
//	freopen( "CON", "w", stdout );
//    freopen( "CON", "w", stderr );
    
    //when the program exits, clean everything up
    atexit(stopSDL);
    
    //initialization stuff
    initEntityCreateTable();
    initEnemyCreateTable();
    initWeaponCreateTable();
    initWeaponLists(); //creates the player's arrays, must be done before the area and player
    initInventory(); //should be done before player
    initPlayer();
    initMenu();
    initInput();
    initTextbox();
    initInterface();
    initTitle();
    initLoadScreen();

    initFrames();
    int newFrame;
    int currFrame = 0;
    int numFrames = 1;
    Frame *frames[15]; //PIZZA - this is hardcoded, just increase to match number of possible frames?  Probably a small number
    frames[0] = _currentFrame;
    
    //dat main loop doe
    while (go){
        //calculate the time delta
        prevMilliseconds = currMilliseconds;
        currMilliseconds = SDL_GetTicks();
        delta = currMilliseconds - prevMilliseconds;
        
        getInput();
        
        newFrame = frames[currFrame]->logic(delta);
        if (newFrame == 1){
            numFrames++;
            currFrame++;
            frames[currFrame] = _currentFrame;
        } else if (newFrame == -1){
            numFrames--;
            currFrame--;
        } else {
            clearScreen();
            for (i = 0; i < numFrames; i++){
                if (i != numFrames-1 && !frames[i]->drawIfNotTop){
                    continue;
                } else {
                    frames[i]->draw();
                }
            }
            bufferToScreen();
        }
        
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
