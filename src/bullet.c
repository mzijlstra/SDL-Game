#include "bullet.h"
#include "asset.h"
#include "linkedlist.h"

extern Assets assets;

void initBullet(Bullet *const bullet, Player *const player) {
    bullet->x = player->location.x;
    bullet->y = player->location.y;
    bullet->dx = player->velocity.x + 5;
    bullet->dy = 0;
    bullet->ttl = 250;
    bullet->img = assets.bullet;
    bullet->imgSrc.x = 0;
    bullet->imgSrc.y = 0;
    bullet->imgSrc.w = 16;
    bullet->imgSrc.h = 16;
    bullet->imgDest.x = player->img.shipSrc.x;
    bullet->imgDest.y = player->img.shipSrc.y;
    bullet->imgDest.w = 16;
    bullet->imgDest.h = 16;
    addLink(&player->bulletList, bullet);
}

int updateBullet(Bullet *const bullet, Level *const lvl) {
    bullet->x += bullet->dx;
    bullet->y += bullet->dy;
    bullet->imgDest.x = bullet->x - lvl->src.x;
    bullet->imgDest.y = bullet->y - lvl->src.y;
    bullet->ttl--;
    if (bullet->ttl <= 0) {
        free(bullet);
        return BULLET_DONE;
    }
    return BULLET_GOING;
}