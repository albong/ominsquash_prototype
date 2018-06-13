#ifndef TEXT_H
#define TEXT_H
#include <stdint.h>
#include <stddef.h>

//the ids are unicode points in decimal
typedef struct Text {
    uint32_t *ids;
    size_t length;
} Text;

typedef enum {
    EN, KO
} LanguageCode;

Text *init_Text(Text *self);
void free_Text(Text *self);
void setCurrentLanguage(LanguageCode code);
const char *getCurrentLanguageString();

#endif