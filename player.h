#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <SDL2/SDL.h>
#include "window.h"

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16

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
    unsigned int max_w;
    unsigned int max_h;
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
    uint8_t up_count;
    uint8_t down_count;
    uint8_t right_count;
} PlayerAnim;

typedef struct PlayerTexture {
    SDL_Texture *ship;
    SDL_Texture *flame;
} PlayerTexture;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerAnim anim;
    PlayerTexture texture;
    SDL_Rect ship;
    SDL_Rect shipDest;
    SDL_Rect flame;
    SDL_Rect flameDest;
} Player;

void initPlayer(Player *player, Window * window);

#endif