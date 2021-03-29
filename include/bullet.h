#ifndef BULLET_INCLUDED
#define BULLET_INCLUDED

#include "window.h"

typedef struct Bullet {
    double x;
    double y
    double dx;
    double dy;
    unsigned int ttl;
    SDL_Texture *img;
    SDL_Rect imgSrc;
    SDL_Rect imgDest;
} Bullet;


#endif