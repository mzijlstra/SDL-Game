#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "window.h"
#include "asset.h"
#include "linkedlist.h"
#include "timing.h"

typedef struct PlayerAction {
    SDL_bool up;
    SDL_bool down;
    SDL_bool left;
    SDL_bool right;
    SDL_bool boost;
    SDL_bool fire;
    SDL_bool fullscreen;
    SDL_bool quit;
} PlayerAction;

typedef struct PlayerLocation {
    double x;
    double y;
} PlayerLocation;

typedef struct PlayerAcceleration {
    double up;
    double down;
    double left;
    double right;
    double boost;
} PlayerAcceleration;

typedef struct PlayerVelocity {
    double x;
    double y;
} PlayerVelocity;

typedef struct PlayerAnim {
    unsigned int upCount;
    unsigned int downCount;
    unsigned int boostCount;
    uint8_t shipFrame;
    uint8_t flameFrame;
} PlayerAnim;

typedef struct PlayerImage {
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Rect shipSrc;
    SDL_Rect flameSrc;
} PlayerImage;

typedef struct PlayerView {
    double x;
    double y;
    int w;
    int h;
    unsigned int pixelSize;
} PlayerView;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerAnim anim;
    PlayerImage img;
    LinkNode bulletList;
    PlayerView view;
} Player;

void initPlayer(Player *player, Window * win);
void updatePlayer(Player *const player);
void setPixelSize(Player *const player, Window *const win, unsigned int size);

#endif