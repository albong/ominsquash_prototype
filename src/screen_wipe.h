#ifndef SCREEN_WIPE_H
#define SCREEN_WIPE_H

int doScreenWipe(unsigned delta);
void drawScreenWipe();

void setScreenWipeCenter(double x, double y);
void setScreenWipeInward(int boolean); //true for inward, false for outward

#endif