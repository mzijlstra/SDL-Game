#include "gfx.h"
#include "bullet.h"

void render(Window *win, Level *lvl) {
    Player *player = lvl->p1;
    const unsigned int pxs = player->view.pixelSize;
    const unsigned int tls = TILE_SIZE * pxs;

    // First clear the renderer
    SDL_SetRenderDrawColor(win->renderer, 200, 200, 200, 255);
    SDL_RenderClear(win->renderer);

    // place player in view
    SDL_Rect shipDest;
    shipDest.x = (player->location.x - lvl->p1->view.x) * pxs;
    shipDest.y = (player->location.y - lvl->p1->view.y) * pxs;
    shipDest.w = tls;
    shipDest.h = tls;

    // scroll horizontal if we're not close to the level edges
    if (player->location.x > win->q1w / pxs &&
        player->location.x < lvl->w - (win->q3w / pxs)) {

        // scroll background
        player->view.x += player->velocity.x;
        // make sure we don't scroll out of bounds
        if (player->view.x < 0) {
            player->view.x = 0;
        }
        if (player->view.x > lvl->w - player->view.w ) {
            player->view.x = lvl->w - player->view.w;
        }

        // if the player isn't near .25 of the view, adjust the view
        int diff = shipDest.x - win->q1w;
        if (abs(shipDest.x - win->q1w) > 10) {
            // make it happen in steps so it's not jarring
            diff /= 10;
            shipDest.x -= diff;
            player->view.x += diff;
        }
    }

    // scroll vertical if we're close to window edges and there is level
    const int edgeBuffer = tls * 5;
    if ((shipDest.y <= edgeBuffer && player->view.y > 0) ||
        (shipDest.y >= win->h - edgeBuffer &&
         player->view.y < lvl->h - win->h / pxs)) {

        player->view.y += player->velocity.y;
        // make sure we don't scroll out of bounds 
        if (player->view.y < 0) {
            player->view.y = 0;
        }
        if (player->view.y > lvl->h - player->view.h) {
            player->view.y = lvl->h - player->view.h;
        }
    }

    // place the exhaust flame
    SDL_Rect flameDest;
    flameDest.x = shipDest.x - (8 * pxs);
    flameDest.y = shipDest.y;
    flameDest.w = tls;
    flameDest.h = tls;

    // Draw the background
    SDL_Rect dst, src;
    dst.w = win->w;
    dst.h = win->h;
    dst.x = dst.y = 0;
    src.w = player->view.w;
    src.h = player->view.h;
    src.x = player->view.x;
    src.y = player->view.y;
    SDL_RenderCopy(win->renderer, lvl->img, &src, &dst);

    // Draw player bullets
    LinkNode *iter = lvl->p1->gun.bulletList.next;
    SDL_Rect bltDest;
    bltDest.w = tls;
    bltDest.h = tls;
    while (iter != &lvl->p1->gun.bulletList) {
        Bullet *blt = (Bullet *)iter->data;
        bltDest.x = (blt->x - player->view.x) * pxs;
        bltDest.y = (blt->y - player->view.y) * pxs;

        SDL_RenderCopy(win->renderer, blt->img, &blt->imgSrc, &bltDest);
        iter = iter->next;
    }

    // Draw the player textures
    SDL_RenderCopy(win->renderer, player->img.ship, &player->img.shipSrc,
                   &shipDest);
    SDL_RenderCopy(win->renderer, player->img.flame, &player->img.flameSrc,
                   &flameDest);

    // Update the screen
    SDL_RenderPresent(win->renderer);
}