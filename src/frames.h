#ifndef FRAMES_H
#define FRAMES_H

typedef struct Frame{
    //-1 for pop, 0 for nothing, 1 for push
    int (* logic)(unsigned delta);
    void (* draw)();
    int drawIfNotTop;
} Frame;

Frame *_currentFrame;

void initFrames();

#endif