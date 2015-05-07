#ifndef COLLISIONS_H
#define COLLISIONS_H

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

void doCollisions();
static void doWallCollisions();
static void doEnemyCollisions();

static int rectangleCollide(CollRect r1, CollRect r2);
static int circleCollide(CollCircle c1, CollCircle c2);
static int rectangleCircleCollide(CollRect r, CollCircle c);

#endif
