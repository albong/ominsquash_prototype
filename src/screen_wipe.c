#include "screen_wipe.h"
#include "graphics.h"
#include "constants.h"

static double centerX = 0;
static double centerY = 0;
static int inward = 1;

int doScreenWipe(unsigned delta){
    
}

void drawScreenWipe(){
    
}

void setScreenWipeCenter(double x, double y){
    centerX = x;
    centerY = y;
}

void setScreenWipeInward(int boolean){
    if (boolean) {
        inward = 1;
    } else {
        inward = 0;
    }
}
