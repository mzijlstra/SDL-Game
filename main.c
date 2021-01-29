#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <string.h>

#include "window.h"
#include "player.h"
#include "timing.h"
#include "events.h"
#include "gfx.h"

// global variable used inside timing.c
SDL_bool show_fps;

static void argFPS(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp("fps", argv[i]) == 0) {
            show_fps = SDL_TRUE;
        }
    }
    show_fps = SDL_FALSE;
}

int main(int argc, char *argv[]) {
    SDL_bool show_fps = SDL_FALSE;
    if (argc > 1) {
        argFPS(argc, argv);
    }

    Window window;
    initWindow(&window);

    Player player;
    initPlayer(&player, &window);

    Timing timing;
    initTiming(&timing);

    while (SDL_TRUE) {
        timing.frame_start = SDL_GetTicks();
        getEvents(&player.action, &player.anim);
        if (player.action.quit) {
            break;
        }
        doUpdates(&player, &window);
        render(&window, &player);
        timeFrame(&timing);
    }
    return 0;
}