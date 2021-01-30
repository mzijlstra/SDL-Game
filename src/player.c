#include "player.h"

void initPlayer(Player *player, Window *window) {
    player->action.up = SDL_FALSE;
    player->action.down = SDL_FALSE;
    player->action.left = SDL_FALSE;
    player->action.right = SDL_FALSE;
    player->action.boost = SDL_FALSE;
    player->action.fullscreen = SDL_FALSE;
    player->action.quit = SDL_FALSE;
    player->location.x = 100.0;
    player->location.y = 100.0;
    player->location.max_w = window->width - TILE_WIDTH;
    player->location.max_h = window->height - TILE_HEIGHT;
    player->acceleration.up = 0.3;
    player->acceleration.down = 0.3;
    player->acceleration.right = 0.3;
    player->acceleration.left = 0.3;
    player->acceleration.boost = 0.7;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->anim.up_count = 0;
    player->anim.down_count = 0;
    player->anim.boost_count = 0;
    player->anim.shipFrame = 2;
    player->anim.flameFrame = 0;
    player->img.ship = loadTexture("assets/ship-sheet.png", window->renderer);
    player->img.flame = loadTexture("assets/flame-sheet.png", window->renderer);
    player->img.shipSrc.x = player->anim.shipFrame * TILE_WIDTH;
    player->img.shipSrc.y = 0;
    player->img.shipSrc.w = TILE_WIDTH;
    player->img.shipSrc.h = TILE_HEIGHT;
    player->img.shipDest.x = player->location.x * window->pixel_size;
    player->img.shipDest.y = player->location.y * window->pixel_size;
    player->img.shipDest.w = TILE_WIDTH * window->pixel_size;
    player->img.shipDest.h = TILE_HEIGHT * window->pixel_size;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_WIDTH;
    player->img.flameSrc.y = 0;
    player->img.flameSrc.w = TILE_WIDTH;
    player->img.flameSrc.h = TILE_HEIGHT;
    player->img.flameDest.x = (player->location.x - 9) * window->pixel_size;
    player->img.flameDest.y = player->location.y * window->pixel_size;
    player->img.flameDest.w = TILE_WIDTH * window->pixel_size;
    player->img.flameDest.h = TILE_HEIGHT * window->pixel_size;
}
