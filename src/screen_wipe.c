#include "screen_wipe.h"
#include "graphics.h"
#include "constants.h"
#include <stdio.h>

static double centerX = 0;
static double centerY = 0;
static int wipeInward = 1;
static unsigned totalDelta = 0;

int doScreenWipe(unsigned delta){
    totalDelta += delta;
    
    if (totalDelta > MILLI_PER_TRANSITION + MILLI_HOLD_TRANSITION){
        totalDelta = 0;
        return -1;
    } else {
        return 0;
    }
}

void drawScreenWipe(){
    double transPercent;
    
    //if we're wiping outward, do the held black screen first
    if (!wipeInward && totalDelta <= MILLI_HOLD_TRANSITION){
        drawFilledRect_T(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0,0,0);
    
    //draw the wipe - WIDTH is greater than height, so just use that
    } else if (wipeInward && totalDelta <= MILLI_PER_TRANSITION){
        transPercent = totalDelta / MILLI_PER_TRANSITION;
        drawFilledRect_T(centerX - SCREEN_WIDTH, 0, SCREEN_WIDTH * transPercent, SCREEN_WIDTH, 0,0,0);
        drawFilledRect_T(centerX + (SCREEN_WIDTH * (1-transPercent)), 0, SCREEN_WIDTH, SCREEN_WIDTH, 0,0,0);
        drawFilledRect_T(0, centerY - SCREEN_WIDTH, SCREEN_WIDTH, SCREEN_WIDTH * transPercent, 0,0,0);
        drawFilledRect_T(0, centerY + (SCREEN_WIDTH * (1-transPercent)), SCREEN_WIDTH, SCREEN_WIDTH, 0,0,0);
    } else if (!wipeInward){
        transPercent = (totalDelta - MILLI_HOLD_TRANSITION) / MILLI_PER_TRANSITION;
        drawFilledRect_T(centerX - SCREEN_WIDTH, 0, SCREEN_WIDTH * (1-transPercent), SCREEN_WIDTH, 0,0,0);
        drawFilledRect_T(centerX + (SCREEN_WIDTH * transPercent), 0, SCREEN_WIDTH, SCREEN_WIDTH, 0,0,0);
        drawFilledRect_T(0, centerY - SCREEN_WIDTH, SCREEN_WIDTH, SCREEN_WIDTH * (1-transPercent), 0,0,0);
        drawFilledRect_T(0, centerY + (SCREEN_WIDTH * transPercent), SCREEN_WIDTH, SCREEN_WIDTH, 0,0,0);
    
    //if we wipe inward, do the held black screen last
    } else { // if (wipeInward && totalDelta <= MILLI_HOLD_TRANSITION + MILLI_PER_TRANSITION){
        drawFilledRect_T(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0,0,0);
    }
}

void setScreenWipeCenter(double x, double y){
    centerX = x;
    centerY = y;
}

void setScreenWipeInward(int boolean){
    if (boolean) {
        wipeInward = 1;
    } else {
        wipeInward = 0;
    }
}
