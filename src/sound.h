#ifndef SOUND_H
#define SOUND_H

#include "SDL2/SDL_mixer.h"

#define NUM_SOUND_CHANNELS 100

//this is essentially the same as sound right now, but we could potentially have music be more complicated, with looping sections?
typedef struct Music{
    Mix_Music *music;
    int volumeAdjust;
} Music;

typedef struct Sound{
    Mix_Chunk *chunk;
    int volumeAdjust;
    int channel; //cannot be used to check if playing, since the channel may have been given away
} Sound;

// OTOH, its probably better just to allocate like
// 100 channels and play sounds on whatever is available,
// logging an error if there are no available channels
// the sound struct then holds what channel its being played
// on so that the sound can be cancelled later
// this is easier, more reliable, and more agnostic to SDL

//volumes should probably be stored in the sound objects and loaded from
//a datafile, to enable use to tweak levels without having to recompile

//WAV files only, MP3 has gaps at the beginnings and ends, no good for loops

//VOLUME:
//Overall, the volume can go up to 128, so to have a 10 notch slider in the menu,
//we'll think of volume in ten chunks of size 12, going up to 120.
//Music/sounds can specify a volume adjustment up to +-6.  Suppose the sound in the menu
//is set to 5, and we have sound that has an adjustment of +3.  Then its SDL volume is
//12*5+3.  This gives us a way to do some balancing of audio without remixing, while
//still having a linear scale for volume from the menu.  It also prevents us from
//unintentionally having no volume for something when the menu volume is set to 1,
//and the adjusted volume causes the SDL volume to be 0 (which could happen if we
//allowed arbitrary adjustments)
//
//I'm worried about the performance cost of setting the volume so often though?  maybe
//we should test to see how it impacts, or examine the source?

//PIZZA
//we could store pointers to the currently playing music and sound to check if a particular sound/music is playing

//Library setup/teardown
void initSound();
void termSound();

//Init
Music *init_Music(Music *self);
Sound *init_Sound(Sound *self);

//Freeing - note, these don't check if the music/sound is stopped
void free_Music(Music *self);
void free_Sound(Sound *self);

//Loading
//Its inconvenient to have an intermediary type like "Image" in graphics, this obscures SDL from data_reader
Music *fillMusicFromFile(Music *self, char *name);
Sound *fillSoundFromFile(Sound *self, char *name);

//Music
void playMusic(Music *music);
void stopMusic();
void fadeOutMusic();
void fadeToMusic(Music *newMusic, int fadeDuration);
int musicIsPlaying();
void increaseMusicVolume();
void decreaseMusicVolume();

//Sounds
int playSound(Sound *sound);
int repeatSound(Sound *sound, int numRepeats);
void stopSound(Sound *sound);
void stopAllSound();
void increaseSoundVolume();
void decreaseSoundVolume();

#endif