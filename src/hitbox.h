#ifndef HITBOX_H
#define HITBOX_H

typedef struct CollRect {
    double x, y, w, h;
} CollRect;

typedef struct CollCircle {
    double cx, cy, r;
} CollCircle;

typedef struct Hitbox{
    int numRect;
    int numCircle;
    CollRect *rects;
    CollCircle *circles;
} Hitbox;

typedef struct Hitboxes { 
    int numMovement;
    int numInteract;
    Hitbox *movement; //array
    Hitbox *interact; //array
} Hitboxes;

#endif
