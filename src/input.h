#ifndef INPUT_H
#define INPUT_H

// Structs
/*
Most of the buttons do more or less as labelled on the tin;
'a' and 'b' are for weapons, 'x' is for interacting, 'y' is undecided but maybe jump?
*/
typedef struct Control {
    int up, down, left, right, a, b, x, y, start, select;
} Control;

// Globals
Control _input;

// Access
void getInput();

#endif
