#include "timing.h"

extern SDL_bool show_fps;

void initTiming(Timing *timing) {
    timing->frame_count = 0;
    timing->secs = 0;
    timing->fps = 0;
    timing->start_time = SDL_GetTicks();
}

void timeFrame(Timing *t) {
    t->frame_stop = SDL_GetTicks();
    unsigned int target_frames = (t->frame_stop - t->start_time) / FRAME_TIME;

    // only pause if we're on track for 100 fps
    if (t->frame_count >= target_frames) {
        while (t->frame_stop - t->frame_start < FRAME_TIME) {
            SDL_Delay(1);
            t->frame_stop = SDL_GetTicks();
        }
    }

    t->frame_count += 1;
    t->fps += 1;
    if ((t->frame_stop - t->start_time) / 1000 > t->secs) {
        t->secs += 1;
        if (show_fps) {
            SDL_Log("FPS: %d\n", t->fps);
        }
        t->fps = 0;
    }
    if (t->frame_stop - t->frame_start > FRAME_TIME + 2) {
        SDL_Log("Big Frame: %d millisecs\n", t->frame_stop - t->frame_start);
    }
}
