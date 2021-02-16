#ifndef STATE_INCLUDED
#define STATE_INCLUDED

#include "window.h"
#include "player.h"
#include "level.h"
#include "timing.h"

typedef struct GameState {
    void (*getEvents)(PlayerAction *, PlayerAnim *);
    void (*doUpdates)(Level *, Window *);
    void (*render)(Window *, Level *);
    void (*timeFrame)(Timing *);
} GameState;

#endif