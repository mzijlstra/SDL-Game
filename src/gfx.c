#include "gfx.h"
#include "bullet.h"

void render(Window *win, Level *lvl) {
    // First clear the renderer
    SDL_SetRenderDrawColor(win->renderer, 200, 200, 200, 255);
    SDL_RenderClear(win->renderer);

    // Draw the background
    SDL_Rect dst;
    dst.w = win->w;
    dst.h = win->h;
    dst.x = dst.y = 0;
    SDL_RenderCopy(win->renderer, lvl->img, &lvl->src, &dst);

    // Draw player bullets 
    LinkNode *iter = lvl->p1->bulletList.next;
    while (iter != &lvl->p1->bulletList) {
        Bullet *blt = (Bullet *)iter->data;
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