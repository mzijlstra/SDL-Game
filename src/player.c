#include "player.h"

void initPlayer(Player *player, Window *win) {
    player->action.up = SDL_FALSE;
    player->action.down = SDL_FALSE;
    player->action.left = SDL_FALSE;
    player->action.right = SDL_FALSE;
    player->action.boost = SDL_FALSE;
    player->action.fullscreen = SDL_FALSE;
    player->action.quit = SDL_FALSE;
    player->location.x = win->w / 2;
    player->location.y = win->h / 2;
    player->acceleration.up = 0.7;
    player->acceleration.down = 0.7;
    player->acceleration.right = 0.7;
    player->acceleration.left = 0.7;
    player->acceleration.boost = 1.3;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->anim.up_count = 0;
    player->anim.down_count = 0;
    player->anim.boost_count = 0;
    player->anim.shipFrame = 2;
    player->anim.flameFrame = 0;
    player->img.ship = loadTexture("assets/ship-sheet.png", win->renderer);
    player->img.flame = loadTexture("assets/flame-sheet.png", win->renderer);
    player->img.shipSrc.x = player->anim.shipFrame * TILE_SIZE;
    player->img.shipSrc.y = 0;
    player->img.shipSrc.w = TILE_SIZE;
    player->img.shipSrc.h = TILE_SIZE;
    player->img.shipDest.x = player->location.x * win->pixel_size;
    player->img.shipDest.y = player->location.y * win->pixel_size;
    player->img.shipDest.w = TILE_SIZE * win->pixel_size;
    player->img.shipDest.h = TILE_SIZE * win->pixel_size;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_SIZE;
    player->img.flameSrc.y = 0;
    player->img.flameSrc.w = TILE_SIZE;
    player->img.flameSrc.h = TILE_SIZE;
    player->img.flameDest.x = (player->location.x - 9) * win->pixel_size;
    player->img.flameDest.y = player->location.y * win->pixel_size;
    player->img.flameDest.w = TILE_SIZE * win->pixel_size;
    player->img.flameDest.h = TILE_SIZE * win->pixel_size;
}
