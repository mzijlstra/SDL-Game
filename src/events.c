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
                lvl->p1->view.w = win->w / win->pixelSize;
                lvl->p1->view.h = win->h / win->pixelSize;
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
            case SDLK_SPACE:
                p->action.fire = SDL_TRUE;
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
            case SDLK_SPACE:
                p->action.fire = SDL_FALSE;
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
    // Window related actions
    Player *player = lvl->p1;
    if (player->action.fullscreen) {
        // Only do it once
        player->action.fullscreen = SDL_FALSE;

        if (win->mode == 0) {
            SDL_SetWindowFullscreen(win->ptr, SDL_WINDOW_FULLSCREEN_DESKTOP);
            win->mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
            win->pixelSize = 4;

            SDL_GetWindowSize(win->ptr, &win->w, &win->h);

            win->pixelSize = 2; // TODO hardcoded

            player->img.shipDest.x *= win->pixelSize;
            player->img.shipDest.y *= win->pixelSize;
            player->img.shipDest.w *= win->pixelSize;
            player->img.shipDest.h *= win->pixelSize;
            player->img.flameDest.x *= win->pixelSize;
            player->img.flameDest.y *= win->pixelSize;
            player->img.flameDest.w *= win->pixelSize;
            player->img.flameDest.h *= win->pixelSize;
        } else if (win->mode == SDL_WINDOW_FULLSCREEN_DESKTOP) {
            SDL_SetWindowFullscreen(win->ptr, 0);
            win->mode = 0;

            win->w = 960; // TODO hardcoded
            win->h = 540; // TODO hardcoded

            player->img.shipDest.x /= win->pixelSize;
            player->img.shipDest.y /= win->pixelSize;
            player->img.shipDest.w /= win->pixelSize;
            player->img.shipDest.h /= win->pixelSize;
            player->img.flameDest.x /= win->pixelSize;
            player->img.flameDest.y /= win->pixelSize;
            player->img.flameDest.w /= win->pixelSize;
            player->img.flameDest.h /= win->pixelSize;

            win->pixelSize = 1; // TODO hardcoded
        }
    }

    // then update the level and everything inside it
    updateLevel(lvl);
}
