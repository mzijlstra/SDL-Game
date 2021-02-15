#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "window.h"

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
    unsigned int up_count;
    unsigned int down_count;
    unsigned int boost_count;
    uint8_t shipFrame;
    uint8_t flameFrame;
} PlayerAnim;

typedef struct PlayerImage {
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Rect shipSrc;
    SDL_Rect shipDest;
    SDL_Rect flameSrc;
    SDL_Rect flameDest;
} PlayerImage;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerAnim anim;
    PlayerImage img;
} Player;

void initPlayer(Player *player, Window * win);

#endif