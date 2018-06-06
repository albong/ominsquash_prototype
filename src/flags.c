#include "flags.h"
#include "logging.h"
#include <stddef.h>
#include <limits.h>

// Why use CHAR_BIT, this won't run anywhere its not 8: SHUT UR MOUTH

static char flags[NUM_FLAGS/CHAR_BIT + 1]; //+1 for safety

void initGlobalFlagTable(){
    size_t i;
    for (i = 0; i < (NUM_FLAGS/CHAR_BIT)+1; i++){
        flags[i] = 0;
    }
}

void termGlobalFlagTable(){
    //this don't really do anything since there's no dynamic allocation
}

//
// We do these with bitwise shifts, but I'm not sure if a switch statement 
// with constant flags might be faster
//
int checkGlobalFlag(size_t id){
    return (flags[id / CHAR_BIT]) & (1 << (id % CHAR_BIT));
}

void setGlobalFlag(size_t id){
    flags[id / CHAR_BIT] |= (1 << (id % CHAR_BIT));
    LOG_INF("Flag %d set", id);
}

void unsetGlobalFlag(size_t id){
    flags[id / CHAR_BIT] &= ~(1 << (id % CHAR_BIT));
    LOG_INF("Flag %d unset", id);
}
