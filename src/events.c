#include "events.h"

void getEvents(Level *lvl, Window *win) {
    Player *p = lvl->p1;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            p->action.quit = SDL_TRUE;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log("Window %d size changed to %dx%d",
                        event.window.windowID, event.window.data1,
                        event.window.data2);

                windowSizeChanged(win, event.window.data1, event.window.data2);
                lvl->src.w = win->w / win->pixel_size;
                lvl->src.h = win->h / win->pixel_size;
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                p->action.quit = SDL_TRUE;
                break;
            case SDLK_f:
                p->action.fullscreen = SDL_TRUE;
                break;
            case SDLK_UP:
            case SDLK_w:
                p->action.up = SDL_TRUE;
                p->anim.up_count = 0;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                p->action.right = SDL_TRUE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                p->action.down = SDL_TRUE;
                p->anim.down_count = 0;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                p->action.left = SDL_TRUE;
                break;
            case SDLK_TAB:
                p->action.boost = SDL_TRUE;
                p->anim.boost_count = 0;
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                p->action.up = SDL_FALSE;
                p->anim.up_count = ANIM_TIME;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                p->action.right = SDL_FALSE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                p->action.down = SDL_FALSE;
                p->anim.down_count = ANIM_TIME;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                p->action.left = SDL_FALSE;
                break;
            case SDLK_TAB:
                p->action.boost = SDL_FALSE;
                p->anim.boost_count = 0;
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        default:
            break;
        }
    }
}

void doUpdates(Level *lvl, Window *win) {
    Player *player = lvl->p1;
    if (player->action.fullscreen) {
        // Only do it once
        player->action.fullscreen = SDL_FALSE;

        if (win->mode == 0) {
            SDL_SetWindowFullscreen(win->ptr, SDL_WINDOW_FULLSCREEN_DESKTOP);
            win->mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
            win->pixel_size = 4;

            SDL_GetWindowSize(win->ptr, &win->w, &win->h);

            win->pixel_size = 2; // TODO hardcoded

            player->img.shipDest.x *= win->pixel_size;
            player->img.shipDest.y *= win->pixel_size;
            player->img.shipDest.w *= win->pixel_size;
            player->img.shipDest.h *= win->pixel_size;
            player->img.flameDest.x *= win->pixel_size;
            player->img.flameDest.y *= win->pixel_size;
            player->img.flameDest.w *= win->pixel_size;
            player->img.flameDest.h *= win->pixel_size;
        } else if (win->mode == SDL_WINDOW_FULLSCREEN_DESKTOP) {
            SDL_SetWindowFullscreen(win->ptr, 0);
            win->mode = 0;

            win->w = 960; // TODO hardcoded
            win->h = 540; // TODO hardcoded

            player->img.shipDest.x /= win->pixel_size;
            player->img.shipDest.y /= win->pixel_size;
            player->img.shipDest.w /= win->pixel_size;
            player->img.shipDest.h /= win->pixel_size;
            player->img.flameDest.x /= win->pixel_size;
            player->img.flameDest.y /= win->pixel_size;
            player->img.flameDest.w /= win->pixel_size;
            player->img.flameDest.h /= win->pixel_size;

            win->pixel_size = 1; // TODO hardcoded
        }
    }

    if (player->action.up) {
        // move ship
        player->velocity.y -= player->acceleration.up;
        // tilt ship
        player->anim.up_count += 1;
        if (!player->anim.down_count && player->action.boost &&
            player->action.right && player->anim.up_count > ANIM_TIME) {
            player->anim.shipFrame = 0;
        } else if (player->anim.down_count) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 1;
        }
    } else {
        if (player->anim.up_count > 0) {
            player->anim.up_count -= 1;
        }
    }

    if (player->action.down) {
        // move ship
        player->velocity.y += player->acceleration.down;
        // tilt ship
        player->anim.down_count += 1;
        if (!player->anim.up_count && player->action.boost &&
            player->action.right && player->anim.down_count > ANIM_TIME) {
            player->anim.shipFrame = 4;
        } else if (player->anim.up_count) {
            player->anim.shipFrame = 2;
        } else {
            player->anim.shipFrame = 3;
        }
    } else {
        if (player->anim.down_count > 0) {
            player->anim.down_count -= 1; // TODO act on this
        }
    }

    if (!player->action.up && !player->action.down) {
        player->anim.shipFrame = 2;
    }

    if (player->action.left) {
        // move ship
        player->velocity.x -= player->acceleration.left;
        // set ship flame
        player->anim.flameFrame = 0;
    } else if (player->action.right) {
        // move ship
        if (player->action.boost) {
            player->velocity.x += player->acceleration.boost;
        } else {
            player->velocity.x += player->acceleration.right;
        }

        // set ship flame
        if (player->action.boost) {
            player->anim.boost_count += 1;
            if (player->anim.boost_count % 10 == 0) {
                // every 100milli secs
                player->anim.flameFrame += 1;
                if (player->anim.flameFrame == 4) {
                    player->anim.flameFrame = 2;
                }
            }
        } else {
            player->anim.flameFrame = 1;
        }
    } else {
        player->anim.flameFrame = 0;
    }

    // adjust animation frames
    player->img.shipSrc.x = player->anim.shipFrame * TILE_SIZE;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_SIZE;

    // update velocity
    player->velocity.x = player->velocity.x * (1.0 - 0.2); // 0.2 is drag
    player->velocity.y = player->velocity.y * (1.0 - 0.2);
    if (fabs(player->velocity.x) < 0.1) {
        player->velocity.x = 0;
    }
    if (fabs(player->velocity.y) < 0.1) {
        player->velocity.y = 0;
    }

    // update player location
    player->location.x += player->velocity.x;
    player->location.y += player->velocity.y;

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

    // place player on screen
    player->img.shipDest.x =
        ((int)player->location.x - lvl->src.x) * win->pixel_size;
    player->img.flameDest.x = player->img.shipDest.x - (8 * win->pixel_size);
    player->img.shipDest.y =
        ((int)player->location.y - lvl->src.y) * win->pixel_size;
    player->img.flameDest.y = player->img.shipDest.y;

    // instead of move player right move level left at 0.25 screen width
    if ((player->img.shipDest.x > win->q1w &&
         player->location.x < lvl->w - win->q1w &&
         player->location.x < lvl->w - win->q3w) ||
        // also when moving left beyond 0.25 screen witdh in the level
        (player->img.shipDest.x < win->q1w && player->location.x > win->q1w)) {

        int diff = player->img.shipDest.x - win->q1w;
        // if it's a big change, make it happen in steps so it's not as jarring
        if (abs(diff) > 5) {
            diff = diff / 5;
        }
        player->img.shipDest.x -= diff;
        player->img.flameDest.x = player->img.shipDest.x - 8;
        lvl->src.x += diff;
    }

    // move level up instead of player down at 0.75 screen height
    if (player->img.shipDest.y > win->q3h &&
        player->location.y < lvl->h - win->q1h) {
        int diff = player->img.shipDest.y - win->q3h;
        player->img.shipDest.y = win->q3h;
        player->img.flameDest.y = win->q3h;
        lvl->src.y += diff;
    }

    // move level down instead of player up at 0.25 screen height
    if (player->img.shipDest.y < win->q1h && player->location.y > win->q1h) {
        int diff = player->img.shipDest.y - win->q1h;
        player->img.shipDest.y = win->q1h;
        player->img.flameDest.y = win->q1h;
        lvl->src.y += diff;
    }
}
