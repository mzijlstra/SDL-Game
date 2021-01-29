#ifndef EVENTS_INCLUDED
#define EVENTS_INCLUDED

#include "player.h"

void getEvents(PlayerAction *action, PlayerAnim *anim);
void doUpdates(Player *player, Window *window);

#endif