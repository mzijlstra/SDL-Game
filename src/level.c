#include "level.h"
#include "bullet.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
unsigned int rmask = 0xff000000;
unsigned int gmask = 0x00ff0000;
unsigned int bmask = 0x0000ff00;
unsigned int amask = 0x000000ff;
#else
unsigned int rmask = 0x000000ff;
unsigned int gmask = 0x0000ff00;
unsigned int bmask = 0x00ff0000;
unsigned int amask = 0xff000000;
#endif

extern Assets assets;

void initLevel(Level *const lvl, Player *const p1, Window *const win) {
    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }

    lvl->w = mode.w;
    lvl->h = mode.h;

    initPlayer(p1, win);
    lvl->p1 = p1;

    // create texture for level
    SDL_Surface *tiles = assets.bg_tiles;
    SDL_Surface *img =
        SDL_CreateRGBSurface(0, mode.w, mode.h, 32, rmask, gmask, bmask, amask);

    SDL_Rect src, dst;
    src.w = src.h = dst.w = dst.h = TILE_SIZE;
    src.x = src.y = dst.x = dst.y = 0;

    // 2 rows of 'top'
    for (int i = 0; i < 2; i++) {
        dst.x = 0;
        while (dst.x < lvl->w) {
            SDL_BlitSurface(tiles, &src, img, &dst);
            dst.x += TILE_SIZE;
        }
        dst.y += TILE_SIZE;
    }

    // 1 row of rocks going into canyon
    src.y += TILE_SIZE;
    dst.x = 0;
    while (dst.x < lvl->w) {
        SDL_BlitSurface(tiles, &src, img, &dst);
        dst.x += TILE_SIZE;
    }
    dst.y += TILE_SIZE;

    // 9 rows of in canyon
    src.y += TILE_SIZE;
    for (int i = 0; i < 9; i++) {
        dst.x = 0;
        while (dst.x < lvl->w) {
            SDL_BlitSurface(tiles, &src, img, &dst);
            dst.x += TILE_SIZE;
        }
        dst.y += TILE_SIZE;
    }

    // 1 row of rocks going out of the canyon
    src.y += TILE_SIZE;
    dst.x = 0;
    while (dst.x < lvl->w) {
        SDL_BlitSurface(tiles, &src, img, &dst);
        dst.x += TILE_SIZE;
    }
    dst.y += TILE_SIZE;

    // fill the remainder with 'top'
    src.y = 0;
    while (dst.y < lvl->h) {
        dst.x = 0;
        while (dst.x < lvl->w) {
            SDL_BlitSurface(tiles, &src, img, &dst);
            dst.x += TILE_SIZE;
        }
        dst.y += TILE_SIZE;
    }

    lvl->img = SDL_CreateTextureFromSurface(win->renderer, img);
    SDL_FreeSurface(tiles);
    SDL_FreeSurface(img);
}

void updateLevel(Level *const lvl) { 
    Player *player = lvl->p1;
    updatePlayer(player);

    // keep player inside the level
    if (player->location.x < 0) {
        player->location.x = 0;
    } else if (player->location.x > lvl->w - TILE_SIZE) {
        player->location.x = lvl->w - TILE_SIZE;
    }
    if (player->location.y < 0) {
        player->location.y = 0;
    } else if (player->location.y > lvl->h - TILE_SIZE) {
        player->location.y = lvl->h - TILE_SIZE;
    }
}