#ifndef ASSET_INCLUDED
#define ASSET_INCLUDED

#include "window.h"
#include "audio.h"

typedef struct Asset {
    SDL_Surface *bgTiles;
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Texture *bullet;
    SDL_Texture *shield;
    WAV shot; 
} Asset;


SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren);
void loadWAV(char *asset, WAV *snd);
void loadAssets(Window* win);
void freeAssets();

#endif