#include "player.h"
#include "bullet.h"

extern Assets assets;

void initPlayer(Player *player, Window *win) {
    player->action.up = SDL_FALSE;
    player->action.down = SDL_FALSE;
    player->action.left = SDL_FALSE;
    player->action.right = SDL_FALSE;
    player->action.boost = SDL_FALSE;
    player->action.fullscreen = SDL_FALSE;
    player->action.quit = SDL_FALSE;
    player->location.x = 25;
    player->location.y = 112;
    player->acceleration.up = 0.3;
    player->acceleration.down = 0.3;
    player->acceleration.right = 0.3;
    player->acceleration.left = 0.3;
    player->acceleration.boost = 1.3;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->anim.up_count = 0;
    player->anim.down_count = 0;
    player->anim.boost_count = 0;
    player->anim.shipFrame = 2;
    player->anim.flameFrame = 0;
    player->img.ship = assets.ship;
    player->img.flame = assets.flame;
    player->img.shipSrc.x = player->anim.shipFrame * TILE_SIZE;
    player->img.shipSrc.y = 0;
    player->img.shipSrc.w = TILE_SIZE;
    player->img.shipSrc.h = TILE_SIZE;
    player->img.shipDest.x = player->location.x * win->pixelSize;
    player->img.shipDest.y = player->location.y * win->pixelSize;
    player->img.shipDest.w = TILE_SIZE * win->pixelSize;
    player->img.shipDest.h = TILE_SIZE * win->pixelSize;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_SIZE;
    player->img.flameSrc.y = 0;
    player->img.flameSrc.w = TILE_SIZE;
    player->img.flameSrc.h = TILE_SIZE;
    player->img.flameDest.x = (player->location.x - 9) * win->pixelSize;
    player->img.flameDest.y = player->location.y * win->pixelSize;
    player->img.flameDest.w = TILE_SIZE * win->pixelSize;
    player->img.flameDest.h = TILE_SIZE * win->pixelSize;
    player->bulletList.next = &player->bulletList;
    player->bulletList.prev = &player->bulletList;
    player->bulletList.data = NULL; // this is the sentinel node
    player->view.x = 0;
    player->view.y = 0;
    player->view.w = win->w / win->pixelSize;
    player->view.h = win->h / win->pixelSize;
}

void updatePlayer(Player *const player) {
    // create a bullet if the player is firing
    if (player->action.fire) {
        Bullet *bullet = malloc(sizeof(Bullet));
        initBullet(bullet, player);
    }

    // update player bullets
    LinkNode *iter = player->bulletList.next;
    LinkNode *next = NULL;
    while (iter != &player->bulletList) {
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
        player->anim.up_count += 1;
        if (!player->anim.down_count && player->action.boost &&
            player->action.right && player->anim.up_count > ANIM_TIME) {
            player->anim.shipFrame = 0;
        } else if (player->anim.down_count) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 1;
        }
    } else {
        if (player->anim.up_count > 0) {
            player->anim.up_count -= 1;
        }
    }

    if (player->action.down) {
        // move ship
        player->velocity.y += player->acceleration.down;
        // tilt ship
        player->anim.down_count += 1;
        if (!player->anim.up_count && player->action.boost &&
            player->action.right && player->anim.down_count > ANIM_TIME) {
            player->anim.shipFrame = 4;
        } else if (player->anim.up_count) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 3;
        }
    } else {
        if (player->anim.down_count > 0) {
            player->anim.down_count -= 1; // TODO act on this
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
            player->anim.boost_count += 1;
            if (player->anim.boost_count % 10 == 0) {
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