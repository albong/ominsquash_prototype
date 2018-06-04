#ifndef SOUND_H
#define SOUND_H

typedef struct Sound{
    int channel;
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

//Initialization
void initSound();
void termSound();

//Loading
Sound *loadSound(char *name);

//Freeing
void freeSound(Sound *self);

//Music
void setMusicVolume(int volume); //can be called while music is playing?
void playMusic(Sound *music);
void stopMusic();
void fadeOutMusic();
void fadeToMusic(Sound *newMusic, int fadeDuration);
void increaseMusicVolume();
void decreaseMusicVolume();

//Sounds
int playSound(Sound *sound, int volume);
int repeatSound(Sound *sound, int volume, int numRepeats);
void stopSound(Sound *sound);
void increaseSoundsVolume();
void decreaseSoundsVolume();

#endif