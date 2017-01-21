#ifndef DATA_READER_H
#define DATA_READER_H

#include "area.h"
#include "entity.h"

Area *readAreaFromFile(char *filename, Area *result);
Entity *readEntityFromFile(char *filename, Entity *result);

#endif