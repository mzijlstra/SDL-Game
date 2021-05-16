#ifndef AUDIO_INCLUDED
#define AUDIO_INCLUDED

#include "window.h"
#include "linkedlist.h"

typedef struct WAV {
    SDL_AudioSpec spec;
    uint32_t len;
    uint8_t *buf;
} WAV;

typedef struct SND {
    SDL_AudioSpec *spec;
    uint8_t *buf;
    uint32_t len;
    uint32_t idx;
} SND;

void initAudio();
void audioCallback(void *userdata, uint8_t *stream, int len);
void playSound(WAV *const wav);


#endif