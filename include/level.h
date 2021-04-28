#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

#include "window.h"
#include "player.h"

typedef struct Level {
    int w;
    int h;
    Player *p1;
    SDL_Texture *img;
    SDL_Rect src;
} Level;

void initLevel(Level *const lvl, Player *const p1, Window *const win);
void updateLevel(Level *const lvl, Window *const win);

#endif