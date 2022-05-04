#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

#include "window.h"
#include "asset.h"
#include "level.h"
#include "player.h"
#include "timing.h"
#include "linkedlist.h"
#include "events.h"
#include "gfx.h"
#include "audio.h"
#include "state.h"

// global variables
SDL_bool showFPS;
Asset asset;

static void argFPS(int argc, char *argv[]) {
    showFPS = SDL_FALSE;
    for (int i = 1; i < argc; i++) {
        if (strcmp("fps", argv[i]) == 0) {
            showFPS = SDL_TRUE;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        argFPS(argc, argv);
    }

    Window win;
    windowConstructor(&win);
    loadAssets(&win);

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
        timing.frameStart = SDL_GetTicks64();
        (*playLvl.getEvents)(&lvl, &win);
        if (p1.action.quit) {
            break;
        }
        (*playLvl.doUpdates)(&lvl, &win);
        (*playLvl.render)(&win, &lvl);
        (*playLvl.timeFrame)(&timing);
    }

    destroyLevel(&lvl);
    freeAssets();
    windowDestructor(&win); 
    return 0;
}