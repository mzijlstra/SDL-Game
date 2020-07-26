#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Window *win =
        SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 384, 320, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *img = IMG_Load("assets/ship.png");
    if (img == NULL) {
        SDL_Log("IMG_Load Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, img);
    if (tex == NULL) {
        SDL_Log("SDL_CreateTexture: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface(img);

    for (int i = 0; i < 3; ++i) {
        // First clear the renderer
        SDL_RenderClear(ren);
        // Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);
        // Update the screen
        SDL_RenderPresent(ren);
        // Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    return 0;
}