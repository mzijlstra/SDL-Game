#include "gfx.h"

void render(Window *window, Player *player) {
    // First clear the renderer
    SDL_SetRenderDrawColor(window->renderer, 200, 200, 200, 255);
    SDL_RenderClear(window->renderer);
    // Draw the textures
    SDL_RenderCopy(window->renderer, player->texture.ship, &player->ship,
                   &player->shipDest);
    SDL_RenderCopy(window->renderer, player->texture.flame, &player->flame,
                   &player->flameDest);

    // Update the screen
    SDL_RenderPresent(window->renderer);
}