#ifndef TIMING_INCLUDED
#define TIMING_INCLUDED

#include <SDL2/SDL.h>

#define FRAME_TIME 10

typedef struct Timing {
    unsigned long frame_count;
    unsigned int secs;
    unsigned int fps;
    unsigned int start_time;
    unsigned int frame_start;
    unsigned int frame_stop;
} Timing;

void initTiming(Timing *timing);
void timeFrame(Timing *t);

#endif