#ifndef FLAGS_H
#define FLAGS_H

#include <stddef.h>

#define NUM_FLAGS 4000 //multiple of CHAR_BIT please

void initGlobalFlagTable();
void termGlobalFlagTable();
int checkGlobalFlag(size_t id); //returns 0 or non-zero
void setGlobalFlag(size_t id);
void unsetGlobalFlag(size_t id);

#endif