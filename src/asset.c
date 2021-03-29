#include "asset.h"

extern Assets assets;

SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren) {
    SDL_Texture *tex = NULL;
    SDL_Surface *img = IMG_Load(asset);
    SDL_bool failed = SDL_FALSE;
    if (img == NULL) {
        SDL_Log("IMG_Load Error: %s\n", SDL_GetError());
        failed = SDL_TRUE;
    } else {
        tex = SDL_CreateTextureFromSurface(ren, img);
        if (tex == NULL) {
            SDL_Log("SDL_CreateTexture: %s\n", SDL_GetError());
            failed = SDL_TRUE;
        }
    }
    SDL_FreeSurface(img);

    // On failure return a placeholder image!
    if (failed) {
        SDL_Log("Creating Placeholder Texture");
        tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 1024, 1024);
        SDL_SetRenderTarget(ren, tex);
        SDL_SetRenderDrawColor(ren, 0x00, 0xff, 0xff, 0xff);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0xff, 0xff);
        SDL_Rect r;
        r.w = 4;
        r.h = 4;
        for (int y = 4; y < 1024; y += 8) {
            for (int x = 4; x < 1024; x += 8) {
                r.x = x;
                r.y = y;
                SDL_RenderFillRect(ren, &r);
            }
        }
        SDL_SetRenderTarget(ren, NULL);
    }

    return tex;
}


void initAssets(Window* win) {
    assets.bg_tiles =  IMG_Load("assets/desert-tiles.png");
    assets.ship = loadTexture("assets/ship-sheet.png", win->renderer);
    assets.flame = loadTexture("assets/flame-sheet.png", win->renderer);
    assets.bullet = loadTexture("assets/bullets.png", win->renderer);
}