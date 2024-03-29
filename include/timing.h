#ifndef TIMING_INCLUDED
#define TIMING_INCLUDED

#include <SDL2/SDL.h>

#define FRAME_TIME 10
#define ANIM_TIME 15

typedef struct Timing {
    unsigned long frameCount;
    unsigned int secs;
    unsigned int fps;
    Uint64 startTime;
    Uint64 frameStart;
    Uint64 frameStop;
} Timing;

void initTiming(Timing *timing);
void timeFrame(Timing *t);

#endif