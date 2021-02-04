#ifndef LEVEL_INCLUDED
#define LEVEL_INCLUDED

#include "window.h"
#include "player.h"

typedef struct Level {
    unsigned int w;
    unsigned int h;
    Player *p1;
    SDL_Texture *img;
} Level;

void initLevel(Level *level, Player *p1, Window *window);

#endif