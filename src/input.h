#ifndef INPUT_H
#define INPUT_H

typedef struct Control {
    int up, down, left, right, a, b, start, select;
} Control;

Control _input;

void getInput();

#endif
