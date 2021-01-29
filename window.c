#include "window.h"

void initWindow(Window *window) {
    window->mode = 0;
    window->pixel_size = 1;
    // TODO set these to 1/4 of desktop resolution
    window->width = 960;
    window->height = 540;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    window->ptr = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, window->width,
                                   window->height, SDL_WINDOW_SHOWN);
    if (window->ptr == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    SDL_Log("Desktop: %d x %d\n", mode.w, mode.h);

    // set these to 1/4 of desktop resolution
    window->width = mode.w / 4;
    window->height = mode.h / 4;

    SDL_SetWindowSize(window->ptr, mode.w / 4, mode.h / 4);
    SDL_Log("Set window size: %d %d\n", mode.w / 4, mode.h / 4);

    window->renderer = SDL_CreateRenderer(
        window->ptr, -1,
        SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC
    if (window->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_DisableScreenSaver();
}

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