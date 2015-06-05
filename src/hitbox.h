#ifndef HITBOX_H
#define HITBOX_H

typedef struct CollRect {
    double x, y, w, h;
} CollRect;

typedef struct CollCircle {
    double cx, cy, r;
} CollCircle;

typedef struct HitBox{
    int numRect;
    int numCircle;
    CollRect *rects;
    CollCircle *circles;
} HitBox;

#endif
