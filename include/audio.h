#ifndef AUDIO_INCLUDED
#define AUDIO_INCLUDED

#include <SDL2/SDL_mixer.h>
#include "window.h"
#include "linkedlist.h"

void initAudio();
void playSound(Mix_Chunk *const snd);

#endif