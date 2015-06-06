#include "hitbox_drawer.h"
#include "../src/graphics.h"
#include "../src/player.h"

void drawHitBoxes(){
    int i;
    int x, y, w, h;
    
    //draw the player's movement box
    if (_player.e.hasMoveHitBox){
        for (i = 0; i < _player.e.moveHitBox[_player.e.currHitBox].numRect; i++){
            x = _player.e.x + _player.e.moveHitBox[_player.e.currHitBox].rects[i].x;
            y = _player.e.y + _player.e.moveHitBox[_player.e.currHitBox].rects[i].y;
            w = _player.e.moveHitBox[_player.e.currHitBox].rects[i].w;
            h =_player.e.moveHitBox[_player.e.currHitBox].rects[i].h;
            drawUnfilledRect(x, y, w, h);
        }
    }
    
    //draw the room's entities
}
