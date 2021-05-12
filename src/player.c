#include "player.h"
#include "bullet.h"

extern Assets assets;

void initPlayer(Player *player, Window *win) {
    player->action.up = SDL_FALSE;
    player->action.down = SDL_FALSE;
    player->action.left = SDL_FALSE;
    player->action.right = SDL_FALSE;
    player->action.boost = SDL_FALSE;
    player->action.fire = SDL_FALSE;
    player->action.fullscreen = SDL_FALSE;
    player->action.quit = SDL_FALSE;
    player->location.x = 25;
    player->location.y = 112;
    player->acceleration.up = 0.3;
    player->acceleration.down = 0.3;
    player->acceleration.right = 0.3;
    player->acceleration.left = 0.3;
    player->acceleration.boost = 1.3;
    player->acceleration.movEnergyUse = 1.0;
    player->acceleration.boostEnergyUse = 5.0;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->anim.upCount = 0;
    player->anim.downCount = 0;
    player->anim.boostCount = 0;
    player->anim.shieldCount = 0;
    player->anim.shipFrame = 2;
    player->anim.flameFrame = 0;
    player->anim.shieldFrame = 0;
    player->img.ship = assets.ship;
    player->img.flame = assets.flame;
    player->img.shield = assets.shield;
    player->img.shipSrc.x = player->anim.shipFrame * TILE_SIZE;
    player->img.shipSrc.y = 0;
    player->img.shipSrc.w = TILE_SIZE;
    player->img.shipSrc.h = TILE_SIZE;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_SIZE;
    player->img.flameSrc.y = 0;
    player->img.flameSrc.w = TILE_SIZE;
    player->img.flameSrc.h = TILE_SIZE;
    player->img.shieldSrc.x = player->anim.shieldFrame * 20;
    player->img.shieldSrc.y = 0;
    player->img.shieldSrc.w = 20;
    player->img.shieldSrc.h = 20;
    player->view.pixelSize = 1;
    player->view.x = 0;
    player->view.y = 0;
    player->view.w = win->w / player->view.pixelSize;
    player->view.h = win->h / player->view.pixelSize;
    player->energy.generate = 1.0;
    player->energy.propulsePercent = 0.34;
    player->energy.shieldPercent = 0.33;
    player->energy.gunPercent = 0.33;
    player->energy.propulseReserve = 250;
    player->energy.shieldReserve = 250;
    player->energy.gunReserve = 250;
    player->shield.targetStrength = 100;
    player->shield.currentStrength = 100;
    player->shield.maintenanceUse = 0.5;
    player->shield.rechargeUse = 2.0;
    player->shield.strengthPerCharge = 0.5;
    player->gun.reloadSpeed = 25;
    player->gun.reloadCount = 0;
    player->gun.energyUsePerShot = 20;
    player->gun.bulletList.next = &player->gun.bulletList;
    player->gun.bulletList.prev = &player->gun.bulletList;
    player->gun.bulletList.data = NULL; // this is the sentinel node
}

void updatePlayer(Player *const player) {
    // update player shield
    player->anim.shieldCount++;
    if (player->anim.shieldCount >= 15) {
        player->anim.shieldFrame = (player->anim.shieldFrame + 1) % 8;
        player->anim.shieldCount = 0;
    }

    // create a bullet if the player is firing
    if (player->action.fire && player->gun.reloadCount == 0) {
        Bullet *bullet = malloc(sizeof(Bullet));
        initBullet(bullet, player);
        player->gun.reloadCount = player->gun.reloadSpeed;
    } else if (player->gun.reloadCount > 0) {
        player->gun.reloadCount--;
    }

    // update player bullets
    LinkNode *iter = player->gun.bulletList.next;
    LinkNode *next = NULL;
    while (iter != &player->gun.bulletList) {
        next = iter->next;
        if (updateBullet(iter->data) == BULLET_DONE) {
            removeLink(iter);
        }
        iter = next;
    }

    if (player->action.up) {
        // move ship
        player->velocity.y -= player->acceleration.up;
        // tilt ship
        player->anim.upCount += 1;
        if (!player->anim.downCount && player->action.boost &&
            player->action.right && player->anim.upCount > ANIM_TIME) {
            player->anim.shipFrame = 0;
        } else if (player->anim.downCount) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 1;
        }
    } else {
        if (player->anim.upCount > 0) {
            player->anim.upCount -= 1;
        }
    }

    if (player->action.down) {
        // move ship
        player->velocity.y += player->acceleration.down;
        // tilt ship
        player->anim.downCount += 1;
        if (!player->anim.upCount && player->action.boost &&
            player->action.right && player->anim.downCount > ANIM_TIME) {
            player->anim.shipFrame = 4;
        } else if (player->anim.upCount) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 3;
        }
    } else {
        if (player->anim.downCount > 0) {
            player->anim.downCount -= 1;
        }
    }

    if (!player->action.up && !player->action.down) {
        player->anim.shipFrame = 2;
    }

    if (player->action.left) {
        // move ship
        player->velocity.x -= player->acceleration.left;
        // set ship flame
        player->anim.flameFrame = 0;
    } else if (player->action.right) {
        // move ship
        if (player->action.boost) {
            player->velocity.x += player->acceleration.boost;
        } else {
            player->velocity.x += player->acceleration.right;
        }

        // set ship flame
        if (player->action.boost) {
            player->anim.boostCount += 1;
            if (player->anim.boostCount % 10 == 0) {
                // every 100milli secs
                player->anim.flameFrame += 1;
                if (player->anim.flameFrame == 4) {
                    player->anim.flameFrame = 2;
                }
            }
        } else {
            player->anim.flameFrame = 1;
        }
    } else {
        player->anim.flameFrame = 0;
    }

    // adjust animation frames
    player->img.shipSrc.x = player->anim.shipFrame * TILE_SIZE;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_SIZE;
    player->img.shieldSrc.x = player->anim.shieldFrame * 20;

    // update velocity
    player->velocity.x = player->velocity.x * (1.0 - 0.2); // 0.2 is drag
    player->velocity.y = player->velocity.y * (1.0 - 0.2);
    if (fabs(player->velocity.x) < 0.1) {
        player->velocity.x = 0;
    }
    if (fabs(player->velocity.y) < 0.1) {
        player->velocity.y = 0;
    }

    // update player location
    player->location.x += player->velocity.x;
    player->location.y += player->velocity.y;
}

void setPixelSize(Player *const player, Window *const win, unsigned int size) {
    if (size < 1 || size > 5) {
        return;
    }

    player->view.pixelSize = size;
    player->view.w = win->w / player->view.pixelSize;
    player->view.h = win->h / player->view.pixelSize;

    // update view.y (player shouldn't disapear off the bottom)
    int bottomView = player->view.y + player->view.h - TILE_SIZE * 5;
    int diff = player->location.y - bottomView;
    if (diff > 0) { // player is below the bottom
        player->view.y += diff;
    } 
    // Is there ever a need to update view.x?
}