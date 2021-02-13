#ifndef EVENTS_INCLUDED
#define EVENTS_INCLUDED

#include "level.h"
#include "timing.h"

void getEvents(PlayerAction *action, PlayerAnim *anim);
void doUpdates(Level *lvl, Window *win);

#endif