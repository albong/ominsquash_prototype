#ifndef TEXT_H
#define TEXT_H
#include <stdint.h>
#include <stddef.h>

//the ids are unicode points in decimal
typedef struct Text {
    uint32_t *ids;
    size_t length;
} Text;

Text *init_Text(Text *self);

#endif