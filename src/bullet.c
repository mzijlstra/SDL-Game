#include "window.h"
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
    bullet->animCount = 10;
    bullet->frame = 0;
    bullet->img = asset.bullet;
    bullet->imgSrc.x = 0;
    bullet->imgSrc.y = 0;
    bullet->imgSrc.w = TILE_SIZE;
    bullet->imgSrc.h = TILE_SIZE;
    addLink(&player->gun.bulletList, bullet);

    // play 'shot' sound
    playSound(asset.shoot);
}

int updateBullet(Bullet *const bullet) {
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    bullet->ttl--;
    if (bullet->ttl <= 0) {
        SDL_free(bullet);
        return BULLET_DONE;
    }
    bullet->animCount--;
    if (bullet->animCount == 0) {
        bullet->animCount = 10;
        bullet->frame = (bullet->frame + 1) % 2;
        bullet->imgSrc.y = TILE_SIZE * bullet->frame;
    }

    return BULLET_GOING;
}