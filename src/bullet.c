#include "bullet.h"
#include "asset.h"
#include "linkedlist.h"
#include "audio.h"

extern Asset asset;

void initBullet(Bullet *const bullet, Player *const player) {
    bullet->x = player->location.x;
    bullet->y = player->location.y;
    bullet->dx = player->velocity.x + 5;
    bullet->dy = 0;
    bullet->ttl = 250;
    bullet->img = asset.bullet;
    bullet->imgSrc.x = 0;
    bullet->imgSrc.y = 0;
    bullet->imgSrc.w = 16;
    bullet->imgSrc.h = 16;
    addLink(&player->gun.bulletList, bullet);

    // play 'shot' sound
    playSound(&asset.shot);
}

int updateBullet(Bullet *const bullet) {
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    bullet->ttl--;
    if (bullet->ttl <= 0) {
        SDL_free(bullet);
        return BULLET_DONE;
    }
    return BULLET_GOING;
}