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
    player->location.max_w = window->width - PLAYER_WIDTH;
    player->location.max_h = window->height - PLAYER_HEIGHT;
    player->acceleration.up = 0.7;
    player->acceleration.down = 0.7;
    player->acceleration.right = 0.7;
    player->acceleration.left = 0.7;
    player->acceleration.boost = 1.3;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->anim.up_count = 0;
    player->anim.down_count = 0;
    player->anim.right_count = 0;
    player->texture.ship = loadTexture("assets/ship.png", window->renderer);
    player->texture.flame = loadTexture("assets/flame.png", window->renderer);
    player->ship.x = 0;
    player->ship.y = 32;
    player->ship.w = PLAYER_WIDTH;
    player->ship.h = PLAYER_HEIGHT;
    player->shipDest.x = player->location.x * window->pixel_size;
    player->shipDest.y = player->location.y * window->pixel_size;
    player->shipDest.w = PLAYER_WIDTH * window->pixel_size;
    player->shipDest.h = PLAYER_HEIGHT * window->pixel_size;
    player->flame.x = 0;
    player->flame.y = 0;
    player->flame.w = 16;
    player->flame.h = 16;
    player->flameDest.x = (player->location.x - 8) * window->pixel_size;
    player->flameDest.y = player->location.y * window->pixel_size;
    player->flameDest.w = PLAYER_WIDTH * window->pixel_size;
    player->flameDest.h = PLAYER_HEIGHT * window->pixel_size;
}
