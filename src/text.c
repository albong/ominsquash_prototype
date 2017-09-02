#include "text.h"

Text *init_Text(Text *self){
    if (self == NULL){
        return NULL;
    }
    
    self->length = 0;
    self->ids = NULL;
    
    return self;
}