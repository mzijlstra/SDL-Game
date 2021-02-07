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
                anim->up_count = 0;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                action->right = SDL_TRUE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                action->down = SDL_TRUE;
                anim->down_count = 0;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                action->left = SDL_TRUE;
                break;
            case SDLK_TAB:
                action->boost = SDL_TRUE;
                anim->boost_count = 0;
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
                anim->up_count = ANIM_TIME;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                action->right = SDL_FALSE;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                action->down = SDL_FALSE;
                anim->down_count = ANIM_TIME;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                action->left = SDL_FALSE;
                break;
            case SDLK_TAB:
                action->boost = SDL_FALSE;
                anim->boost_count = 0;
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

void doUpdates(Player *player, Window *win) {
    if (player->action.fullscreen) {
        // Only do it once
        player->action.fullscreen = SDL_FALSE;

        if (win->mode == 0) {
            SDL_SetWindowFullscreen(win->ptr, SDL_WINDOW_FULLSCREEN_DESKTOP);
            win->mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
            win->pixel_size = 4;

            int w, h;
            SDL_GetWindowSize(win->ptr, &w, &h);
            SDL_Log("Window size: %d %d\n", w, h);

            win->pixel_size = w / win->width;

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

            player->img.shipDest.x /= win->pixel_size;
            player->img.shipDest.y /= win->pixel_size;
            player->img.shipDest.w /= win->pixel_size;
            player->img.shipDest.h /= win->pixel_size;
            player->img.flameDest.x /= win->pixel_size;
            player->img.flameDest.y /= win->pixel_size;
            player->img.flameDest.w /= win->pixel_size;
            player->img.flameDest.h /= win->pixel_size;

            win->pixel_size = 1;
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

    // calculate on screen locations
    player->location.y += player->velocity.y;
    if (player->location.y < 0) {
        player->location.y = 0;
    } else if (player->location.y > player->location.max_h) {
        player->location.y = player->location.max_h;
    }
    player->img.shipDest.y = ((int)player->location.y) * win->pixel_size;
    player->img.flameDest.y = player->img.shipDest.y;

    player->location.x += player->velocity.x;
    if (player->location.x < 0) {
        player->location.x = 0;
    } else if (player->location.x > player->location.max_w) {
        player->location.x = player->location.max_w;
    }
    player->img.shipDest.x = ((int)player->location.x) * win->pixel_size;
    player->img.flameDest.x = ((int)player->location.x - 8) * win->pixel_size;
}
