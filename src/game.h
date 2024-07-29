
#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

// window related code
#define TILE_SIZE 16

typedef struct Window {
    unsigned int mode;
    int w;
    int h;
    int q1w; // left quarter
    int q3w; // right quarter
    int q1h; // end of top quarter 
    int q3h; // begin of bottom quarter
    SDL_Window *ptr;
    SDL_Renderer *renderer;
} Window;

void initWindow(Window *const win);
void windowSizeChanged(Window *const win, int w, int h);
void destroyWindow(Window *const win);

// linked list related code
typedef struct LinkNode {
    struct LinkNode *next;
    struct LinkNode *prev;
    void *data;
} LinkNode;

/* This assumes a cirular linked list with a sentinel node */
void addLink(LinkNode *sentinel, void *data);
/* This assumes that the data in the node has been freed already */
void removeLink(LinkNode *node);

// asset related code
typedef struct Asset {
    SDL_Surface *bgTiles;
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Texture *bullet;
    SDL_Texture *shield;
    Mix_Chunk *shoot; 
} Asset;

SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren);
void loadAssets(Window* win);
void freeAssets();

// audio related code
void initAudio();
void playSound(Mix_Chunk *const snd);

// timing related code
#define FRAME_TIME 10
#define ANIM_TIME 15

typedef struct Timing {
    unsigned long frameCount;
    unsigned int secs;
    unsigned int fps;
    unsigned int startTime;
    unsigned int frameStart;
    unsigned int frameStop;
} Timing;

void initTiming(Timing *timing);
void timeFrame(Timing *t);

// player related code
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
    double movEnergyUse;
    double boostEnergyUse;
} PlayerAcceleration;

typedef struct PlayerVelocity {
    double x;
    double y;
} PlayerVelocity;

typedef struct PlayerAnim {
    unsigned int upCount;
    unsigned int downCount;
    unsigned int boostCount;
    unsigned int shieldCount;
    uint8_t shipFrame;
    uint8_t flameFrame;
    uint8_t shieldFrame;
} PlayerAnim;

typedef struct PlayerImage {
    SDL_Texture *ship;
    SDL_Texture *flame;
    SDL_Texture *shield;
    SDL_Rect shipSrc;
    SDL_Rect flameSrc;
    SDL_Rect shieldSrc;
} PlayerImage;

typedef struct PlayerView {
    double x;
    double y;
    int w;
    int h;
    unsigned int pixelSize;
} PlayerView;

typedef struct PlayerEnergy {
    double generate;
    double propulsePercent;
    double shieldPercent;
    double gunPercent;
    double propulseReserve;
    double shieldReserve;
    double gunReserve;
} PlayerEnergy;

typedef struct PlayerShield {
    double targetStrength;
    double currentStrength;
    double maintenanceUse;
    double rechargeUse;
    double strengthPerCharge;
} PlayerShield;

typedef struct PlayerGun {
    LinkNode bulletList;
    unsigned int reloadSpeed;
    unsigned int reloadCount;
    double energyUsePerShot;
} PlayerGun;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerAnim anim;
    PlayerImage img;
    PlayerView view;
    PlayerEnergy energy;
    PlayerShield shield;
    PlayerGun gun;
} Player;

void initPlayer(Player *player, Window * win);
void updatePlayer(Player *const player);
void setPixelSize(Player *const player, Window *const win, unsigned int size);

// level related code
typedef struct Level {
    int w;
    int h;
    Player *p1;
    SDL_Texture *img;
} Level;

void initLevel(Level *const lvl, Player *const p1, Window *const win);
void updateLevel(Level *const lvl);
void destroyLevel(Level *const lvl);

// bullet related code
#define BULLET_GOING 0
#define BULLET_DONE 1

typedef struct Bullet {
    double x;
    double y;
    double dx;
    double dy;
    unsigned int ttl;
    unsigned int animCount;
    uint8_t frame;
    SDL_Texture *img;
    SDL_Rect imgSrc;
} Bullet;

void initBullet(Bullet *const bullet, Player *const player);
int updateBullet(Bullet *const bullet);

// event related code
void getEvents(Level *lvl, Window *win);
void doUpdates(Level *lvl, Window *win);

// render gfx related code
void render(Window *win, Level *lvl);

// game state related code
typedef struct GameState {
    void (*getEvents)(Level *, Window *);
    void (*doUpdates)(Level *, Window *);
    void (*render)(Window *, Level *);
    void (*timeFrame)(Timing *);
} GameState;

#endif
