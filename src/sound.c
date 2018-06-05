#include "sound.h"
#include "logging.h"
#include "SDL2/SDL_mixer.h"
#include <stdlib.h>


static int musicVolume = 10;
static int soundVolume = 10;

/////////////////////////////////////////////////
// Library setup/teardown
/////////////////////////////////////////////////
void initSound(){
    //Mix_Init(...) will load the dynamic libraries specified by the flags in its argument
    //however, these will be loaded anyway if you try to load something
    //since we're only loading wav files, there's no need to check that we can support other file types
    
    //some mucking with parameters will be needed, see LazyFoo and the documentation for ideas
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        LOG_ERR("SDL_Mixer could not be started: %s", Mix_GetError());
        exit(1);
    }
    Mix_AllocateChannels(NUM_SOUND_CHANNELS); //from the documentation - this call never fails, only segfaults
    LOG_INF("SDL_Mix setup");
}

void termSound(){
    //only need to call Mix_Quit() if we call Mix_Init(...)
    
    //this should be called as many times as Mix_OpenAudio(...) is called; in theory just once
    Mix_CloseAudio();
    LOG_INF("SDL_mixer stopped");
}


/////////////////////////////////////////////////
// Init
/////////////////////////////////////////////////
Music *init_Music(Music *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->music = NULL;
    self->volumeAdjust = 0;
    
    LOG_INF("Music at %p initialized", self);
    return self;
}

Sound *init_Sound(Sound *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    self->chunk = NULL;
    self->volumeAdjust = 0;
    self->channel = -1;
    
    LOG_INF("Sound at %p initialized", self);
    return self;
}


/////////////////////////////////////////////////
// Free
/////////////////////////////////////////////////
void free_Music(Music *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    Mix_FreeMusic(self->music);
    self->music = NULL;
    self->volumeAdjust = 0;
    
    free(self);
    LOG_INF("Music at %p freed", self);
}

void free_Sound(Sound *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    Mix_FreeChunk(self->chunk);
    self->chunk = NULL;
    self->volumeAdjust = 0;
    self->channel = -1;
    
    free(self);
    LOG_INF("Sound at %p freed", self);
}


/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
Music *fillMusicFromFile(Music *self, char *name){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    //check for leaks
    if (self->music != NULL){
        LOG_ERR("Leaking the music from %p", self);
    }
    
    //read in the wav file
    if (name == NULL){
        self->music = NULL;
        LOG_WAR("Received null pointer");
    } else {
        self->music = Mix_LoadMUS(name);
        if (self->music == NULL){
            LOG_ERR("Music in %s could not be loaded: %s", name, Mix_GetError());
        }
    }
    
    LOG_INF("Music at %s filled into %p", name, self);
    return self;
}

Sound *fillSoundFromFile(Sound *self, char *name){
    if (self == NULL || name == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    //check for leaks
    if (self->chunk != NULL){
        LOG_ERR("Leaking the sound from %p", self);
    }
    
    //read in the wav file
    if (name == NULL){
        self->chunk = NULL;
        LOG_WAR("Received null pointer");
    } else {
        self->chunk = Mix_LoadWAV(name);
        if (self->chunk == NULL){
            LOG_ERR("Sound in %s could not be loaded: %s", name, Mix_GetError());
        }
    }
    
    LOG_INF("Sound at %s filled into %p", name, self);
    return self;
}


/////////////////////////////////////////////////
// Music
/////////////////////////////////////////////////
void playMusic(Music *music){
    //first set the volume
    Mix_VolumeMusic(musicVolume * 12 + music->volumeAdjust);
    Mix_PlayMusic(music->music, -1);
    LOG_INF("Music %p playing", music);
}

void stopMusic(){
    Mix_HaltMusic();
    LOG_INF("Music stopped");
}

int musicIsPlaying(){
    return Mix_PlayingMusic(); //0 if not, 1 if yes
}

void increaseMusicVolume(){
    if (musicVolume < 10){
        musicVolume++;
    }
}

void decreaseMusicVolume(){
    if (musicVolume > 0){
        musicVolume--;
    }
}


/////////////////////////////////////////////////
// Sounds
/////////////////////////////////////////////////






