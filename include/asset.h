#ifndef ASSET_INCLUDED
#define ASSET_INCLUDED

#include "window.h"

typedef struct Sound {
    SDL_AudioSpec wavSpec;
    uint32_t wavLength;
    uint8_t *wavBuffer;
} Sound;

typedef struct Assets {
    SDL_Surface *bgTiles;
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Texture *bullet;
    SDL_Texture *shield;
    SDL_AudioDeviceID audioDeviceId; // this is not a great place for it
    Sound shot;
} Assets;


SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren);
void loadSound(char *asset, Sound *snd);
void initAssets(Window* win);
void freeAssets();

#endif