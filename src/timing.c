#include "timing.h"

extern SDL_bool show_fps;

void initTiming(Timing *timing) {
    timing->frameCount = 0;
    timing->secs = 0;
    timing->fps = 0;
    timing->startTime = SDL_GetTicks();
}

void timeFrame(Timing *t) {
    t->frameStop = SDL_GetTicks();
    unsigned int target_frames = (t->frameStop - t->startTime) / FRAME_TIME;

    // only pause if we're on track for 100 fps
    if (t->frameCount >= target_frames) {
        while (t->frameStop - t->frameStart < FRAME_TIME) {
            SDL_Delay(1);
            t->frameStop = SDL_GetTicks();
        }
    }

    t->frameCount += 1;
    t->fps += 1;
    if ((t->frameStop - t->startTime) / 1000 > t->secs) {
        t->secs += 1;
        if (show_fps) {
            SDL_Log("FPS: %d\n", t->fps);
        }
        t->fps = 0;
    }
    if (t->frameStop - t->frameStart > FRAME_TIME + 2) {
        SDL_Log("Big Frame: %d millisecs\n", t->frameStop - t->frameStart);
    }
}
