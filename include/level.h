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

void initLevel(Level *level, Player *p1, Window *window);

#endif