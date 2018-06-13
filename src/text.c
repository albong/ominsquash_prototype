#include "text.h"
#include "logging.h"
#include <stddef.h>
#include <stdlib.h>

static LanguageCode currentLanguageCode = EN;

Text *init_Text(Text *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->length = 0;
    self->ids = NULL;
    
    LOG_INF("Text at %p initialized", self);
    return self;
}

void free_Text(Text *self){
    free(self->ids);
    free(self);
    LOG_INF("Text at %p freed", self);
}

void setCurrentLanguage(LanguageCode code){
    currentLanguageCode = code;
}

const char *getCurrentLanguageString(){
    char *result;
    
    switch (currentLanguageCode){
        case EN:
            return "en";
            break;
        
        case KO:
            return "ko";
            break;
        
        default:
            return "";
            break;
    }
}