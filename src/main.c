#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <string.h>

#include "window.h"
#include "asset.h"
#include "level.h"
#include "player.h"
#include "timing.h"
#include "events.h"
#include "gfx.h"
#include "state.h"

// global variables
SDL_bool show_fps;
Assets assets;

static void argFPS(int argc, char *argv[]) {
    show_fps = SDL_FALSE;
    for (int i = 1; i < argc; i++) {
        if (strcmp("fps", argv[i]) == 0) {
            show_fps = SDL_TRUE;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        argFPS(argc, argv);
    }

    Window win;
    initWindow(&win);
    initAssets(&win);

    Level lvl;
    Player p1;
    initLevel(&lvl, &p1, &win);

    GameState playLvl;
    playLvl.getEvents = &getEvents;
    playLvl.doUpdates = &doUpdates;
    playLvl.render = &render;
    playLvl.timeFrame = &timeFrame;

    Timing timing;
    initTiming(&timing);

    while (SDL_TRUE) {
        timing.frame_start = SDL_GetTicks();
        (*playLvl.getEvents)(&lvl, &win);
        if (p1.action.quit) {
            break;
        }
        (*playLvl.doUpdates)(&lvl, &win);
        (*playLvl.render)(&win, &lvl);
        (*playLvl.timeFrame)(&timing);
    }
    return 0;
}