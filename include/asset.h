#ifndef ASSET_INCLUDED
#define ASSET_INCLUDED

#include "window.h"

typedef struct Assets {
    SDL_Surface *bgTiles;
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Texture *bullet;

} Assets;


SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren);
void initAssets(Window* win);

#endif