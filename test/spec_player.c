
#include "player.h"
#include <SDL2/SDL.h>
#include "bdd-for-c.h"

// global variable needed by player and bullet
Asset asset;

spec("Player Functions") {

    static Window win;
    static Player player;

    before() {
        win.w = 800;
        win.h = 600;
    }

    describe("initPlayer") {
        it("should set all player actions to false") {
            initPlayer(&player, &win);
            check(player.action.up == SDL_FALSE);
            check(player.action.down == SDL_FALSE);
            check(player.action.left == SDL_FALSE);
            check(player.action.right == SDL_FALSE);
            check(player.action.boost == SDL_FALSE);
            check(player.action.fire == SDL_FALSE);
            check(player.action.fullscreen == SDL_FALSE);
            check(player.action.quit == SDL_FALSE);
        }
    }

    describe("updatePlayer") {

    }

    describe("setPixelSize") {

    }
}