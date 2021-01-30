#include "events.h"

void getEvents(PlayerAction *action, PlayerAnim *anim) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            action->quit = SDL_TRUE;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                action->quit = SDL_TRUE;
                break;
            case SDLK_f:
                action->fullscreen = SDL_TRUE;
                break;
            case SDLK_UP:
            case SDLK_w:
                action->up = SDL_TRUE;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                action->right = SDL_TRUE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                action->down = SDL_TRUE;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                action->left = SDL_TRUE;
                break;
            case SDLK_TAB:
                action->boost = SDL_TRUE;
                break;
            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                action->up = SDL_FALSE;
                anim->up_count = 0;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                action->right = SDL_FALSE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                action->down = SDL_FALSE;
                anim->down_count = 0;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                action->left = SDL_FALSE;
                break;
            case SDLK_TAB:
                anim->right_count = 0;
                action->boost = SDL_FALSE;
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

void doUpdates(Player *player, Window *window) {
    if (player->action.fullscreen) {
        // Only do it once
        player->action.fullscreen = SDL_FALSE;

        if (window->mode == 0) {
            SDL_SetWindowFullscreen(window->ptr, SDL_WINDOW_FULLSCREEN_DESKTOP);
            window->mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
            window->pixel_size = 4;

            int w, h;
            SDL_GetWindowSize(window->ptr, &w, &h);
            SDL_Log("Window size: %d %d\n", w, h);

            window->pixel_size = w / window->width;

            player->img.shipDest.x *= window->pixel_size;
            player->img.shipDest.y *= window->pixel_size;
            player->img.shipDest.w *= window->pixel_size;
            player->img.shipDest.h *= window->pixel_size;
            player->img.flameDest.x *= window->pixel_size;
            player->img.flameDest.y *= window->pixel_size;
            player->img.flameDest.w *= window->pixel_size;
            player->img.flameDest.h *= window->pixel_size;
        } else if (window->mode == SDL_WINDOW_FULLSCREEN_DESKTOP) {
            SDL_SetWindowFullscreen(window->ptr, 0);
            window->mode = 0;

            player->img.shipDest.x /= window->pixel_size;
            player->img.shipDest.y /= window->pixel_size;
            player->img.shipDest.w /= window->pixel_size;
            player->img.shipDest.h /= window->pixel_size;
            player->img.flameDest.x /= window->pixel_size;
            player->img.flameDest.y /= window->pixel_size;
            player->img.flameDest.w /= window->pixel_size;
            player->img.flameDest.h /= window->pixel_size;

            window->pixel_size = 1;
        }
    }

    if (player->action.up) {
        // move ship
        player->velocity.y -= player->acceleration.up;
        // tilt ship
        player->anim.up_count += 1;
        if (player->action.boost && player->action.right &&
            player->anim.up_count > 15) {
            player->anim.shipFrame = 0;
        } else {
            player->anim.shipFrame = 1;
        }
    } else if (player->action.down) {
        // move ship
        player->velocity.y += player->acceleration.down;
        // tilt ship
        player->anim.down_count += 1;
        if (player->action.boost && player->action.right &&
            player->anim.down_count > 15) {
            player->anim.shipFrame = 4;
        } else {
            player->anim.shipFrame = 3;
        }
    } else {
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
            player->anim.right_count += 1;
            if (player->anim.right_count % 10 == 0) { 
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
    player->img.shipSrc.x = player->anim.shipFrame * TILE_WIDTH;
    player->img.flameSrc.x = player->anim.flameFrame * TILE_WIDTH;

    // update velocity
    player->velocity.x = player->velocity.x * (1.0 - 0.2); // 0.2 is drag
    player->velocity.y = player->velocity.y * (1.0 - 0.2);
    if (fabs(player->velocity.x) < 0.1) {
        player->velocity.x = 0;
    }
    if (fabs(player->velocity.y) < 0.1) {
        player->velocity.y = 0;
    }

    // calculate on screen locations
    player->location.y += player->velocity.y;
    if (player->location.y < 0) {
        player->location.y = 0;
    } else if (player->location.y > player->location.max_h) {
        player->location.y = player->location.max_h;
    }
    player->img.shipDest.y = ((int)player->location.y) * window->pixel_size;
    player->img.flameDest.y = player->img.shipDest.y;

    player->location.x += player->velocity.x;
    if (player->location.x < 0) {
        player->location.x = 0;
    } else if (player->location.x > player->location.max_w) {
        player->location.x = player->location.max_w;
    }
    player->img.shipDest.x = ((int)player->location.x) * window->pixel_size;
    player->img.flameDest.x = ((int)player->location.x - 8) * window->pixel_size;
}

