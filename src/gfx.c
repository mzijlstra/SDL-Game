#include "gfx.h"
#include "bullet.h"

void render(Window *win, Level *lvl) {
    // TODO make + and - keys change pixelSize
    // then refactor how pixelSize interacts with view
    // and generally refactor all the code below

    // place player in view
    Player *player = lvl->p1;
    player->img.shipDest.x =
        ((int)player->location.x - lvl->p1->view.x) * win->pixelSize;
    player->img.flameDest.x = player->img.shipDest.x - (8 * win->pixelSize);
    player->img.shipDest.y =
        ((int)player->location.y - lvl->p1->view.y) * win->pixelSize;
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
        lvl->p1->view.x += diff;
    }

    // move level up instead of player down at 0.75 screen height
    if (player->img.shipDest.y > win->q3h &&
        player->location.y < lvl->h - win->q1h) {
        int diff = player->img.shipDest.y - win->q3h;
        player->img.shipDest.y = win->q3h;
        player->img.flameDest.y = win->q3h;
        lvl->p1->view.y += diff;
    }

    // move level down instead of player up at 0.25 screen height
    if (player->img.shipDest.y < win->q1h && player->location.y > win->q1h) {
        int diff = player->img.shipDest.y - win->q1h;
        player->img.shipDest.y = win->q1h;
        player->img.flameDest.y = win->q1h;
        lvl->p1->view.y += diff;
    }

    // First clear the renderer
    SDL_SetRenderDrawColor(win->renderer, 200, 200, 200, 255);
    SDL_RenderClear(win->renderer);

    // Draw the background
    SDL_Rect dst;
    dst.w = win->w / win->pixelSize;
    dst.h = win->h / win->pixelSize;
    dst.x = dst.y = 0;
    SDL_RenderCopy(win->renderer, lvl->img, &lvl->p1->view, &dst);

    // Draw player bullets
    LinkNode *iter = lvl->p1->bulletList.next;
    while (iter != &lvl->p1->bulletList) {
        Bullet *blt = (Bullet *)iter->data;
        blt->imgDest.x = (blt->x - player->view.x) / win->pixelSize;
        blt->imgDest.y = (blt->y - player->view.y) / win->pixelSize;

        SDL_RenderCopy(win->renderer, blt->img, &blt->imgSrc, &blt->imgDest);
        iter = iter->next;
    }

    // Draw the player textures
    SDL_RenderCopy(win->renderer, lvl->p1->img.ship, &lvl->p1->img.shipSrc,
                   &lvl->p1->img.shipDest);
    SDL_RenderCopy(win->renderer, lvl->p1->img.flame, &lvl->p1->img.flameSrc,
                   &lvl->p1->img.flameDest);

    // Update the screen
    SDL_RenderPresent(win->renderer);
}