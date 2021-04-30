#ifndef BULLET_INCLUDED
#define BULLET_INCLUDED

#include "level.h"
#include "player.h"

#define BULLET_GOING 0
#define BULLET_DONE 1

typedef struct Bullet {
    double x;
    double y;
    double dx;
    double dy;
    unsigned int ttl;
    SDL_Texture *img;
    SDL_Rect imgSrc;
    SDL_Rect imgDest;
} Bullet;

void initBullet(Bullet *const bullet, Player *const player);
int updateBullet(Bullet *const bullet);

#endif