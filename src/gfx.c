#include "gfx.h"

void render(Window *window, Player *player) {
    // First clear the renderer
    SDL_SetRenderDrawColor(window->renderer, 200, 200, 200, 255);
    SDL_RenderClear(window->renderer);
    // Draw the textures
    SDL_RenderCopy(window->renderer, player->img.ship, &player->img.shipSrc,
                   &player->img.shipDest);
    SDL_RenderCopy(window->renderer, player->img.flame, &player->img.flameSrc,
                   &player->img.flameDest);

    // Update the screen
    SDL_RenderPresent(window->renderer);
}