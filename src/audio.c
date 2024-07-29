#include "game.h"

void initAudio() {
    Mix_Init(0);

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 1024) != 0) {
        SDL_Log("Failed to open audio %s", Mix_GetError());
        exit(1);
    }

    // channels for player sounds
    Mix_AllocateChannels(10);
    Mix_GroupChannels(0, 9, 1);
}

void playSound(Mix_Chunk *const snd) {
    if (Mix_PlayChannel(-1, snd, 0) == -1) {
        SDL_Log("Sound could not be played %s", SDL_GetError());
    }
}
