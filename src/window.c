#include "window.h"
#include "asset.h"

extern Assets assets;

void initWindow(Window *const win) {
    win->mode = 0;
    // Start with basic size everyone should be able to display
    win->w = 640;
    win->h = 480;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    win->ptr = SDL_CreateWindow("Hover Gun Racer", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, win->w, win->h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win->ptr == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    SDL_Log("Desktop: %dx%d\n", mode.w, mode.h);

    // set window size to 1/2 of desktop resolution
    windowSizeChanged(win, mode.w / 2, mode.h / 2);
    SDL_SetWindowSize(win->ptr, win->w, win->h);
    SDL_Log("Set win size: %dx%d\n", win->w, win->h);

    // set window position to be in the middle of the desktop again
    SDL_SetWindowPosition(win->ptr, mode.w / 4, mode.h / 4);

    win->renderer = SDL_CreateRenderer(
        win->ptr, -1,
        SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC
    if (win->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_DisableScreenSaver();

    SDL_AudioSpec want;
    uint8_t *wavBuffer;
    uint32_t wavLength;
    SDL_LoadWAV("assets/shot.wav", &want, &wavBuffer, &wavLength);
    SDL_FreeWAV(wavBuffer);

    assets.audioDeviceId = SDL_OpenAudioDevice(NULL, 0, &want, NULL,
                                               SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (assets.audioDeviceId == 0) {
        SDL_Log("Failed to open audio %s", SDL_GetError());
        exit(1);
    }
    SDL_PauseAudioDevice(assets.audioDeviceId, 0);
}

void windowSizeChanged(Window *const win, int w, int h) {
    win->w = w;
    win->h = h;
    win->q1w = win->w * 0.25;
    win->q3w = win->w - win->q1w;
    win->q1h = win->h * 0.25;
    win->q3h = win->h - win->q1h;
}

void destroyWindow(Window *const win) {
    SDL_CloseAudio();
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->ptr);
}