#ifndef INPUT_H
#define INPUT_H

// Structs
typedef struct Control {
    int up, down, left, right, a, b, start, select;
} Control;

// Globals
Control _input;

// Access
void getInput();

#endif
