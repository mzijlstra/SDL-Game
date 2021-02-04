#include "level.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    unsigned int rmask = 0xff000000;
    unsigned int gmask = 0x00ff0000;
    unsigned int bmask = 0x0000ff00;
    unsigned int amask = 0x000000ff;
#else
    unsigned int rmask = 0x000000ff;
    unsigned int gmask = 0x0000ff00;
    unsigned int bmask = 0x00ff0000;
    unsigned int amask = 0xff000000;
#endif

void initLevel(Level *lvl, Player *p1, Window *window) {
    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    lvl->w = mode.w;
    lvl->h = mode.h;

    initPlayer(p1, window);
    lvl->p1 = p1;

    // TODO create texture for level
    SDL_Surface *tiles = IMG_Load("assets/desert-tiles.png");
    SDL_Surface *img = SDL_CreateRGBSurface(0, mode.w, mode.h, 32,
                                   rmask, gmask, bmask, amask);


}