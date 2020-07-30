#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 270
#define PLAYER_WIDTH 24
#define PLAYER_HEIGHT 16
#define PLAYER_FLAME_FRAMES 2

typedef struct PlayerAction {
    SDL_bool up;
    SDL_bool down;
    SDL_bool left;
    SDL_bool right;
    SDL_bool fire;
} PlayerAction;

typedef struct Player {
    PlayerAction action;
    SDL_Rect sprite;
    SDL_Rect location;
} Player;




int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                       SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC
    if (ren == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *img = IMG_Load("assets/ship.png");
    if (img == NULL) {
        SDL_Log("IMG_Load Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, img);
    if (tex == NULL) {
        SDL_Log("SDL_CreateTexture: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface(img);

    Player player;
    player.action.up = SDL_FALSE;
    player.action.down = SDL_FALSE;
    player.action.left = SDL_FALSE;
    player.action.right = SDL_FALSE;
    player.sprite.x = 24;
    player.sprite.y = 32;
    player.sprite.w = PLAYER_WIDTH;
    player.sprite.h = PLAYER_HEIGHT;
    player.location.x = 100;
    player.location.y = 100;
    player.location.w = PLAYER_WIDTH;
    player.location.h = PLAYER_HEIGHT;

    SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
    SDL_bool running = SDL_TRUE;
    unsigned int frame_time = 10;
    unsigned int time = SDL_GetTicks();
    unsigned int second = time + 1000;
    unsigned int frame_count = 0;
    unsigned int start_tick, stop_tick;
    while (running) {
        start_tick = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = SDL_FALSE;
                    break;

                case SDLK_UP:
                case SDLK_w:
                    player.action.up = SDL_TRUE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    player.action.right = SDL_TRUE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    player.action.down = SDL_TRUE;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    player.action.left = SDL_TRUE;
                    break;
                default:
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    player.action.up = SDL_FALSE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    player.action.right = SDL_FALSE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    player.action.down = SDL_FALSE;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    player.action.left = SDL_FALSE;
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

        if (player.action.up) {
            player.location.y -= 1;
            if (player.location.y < 0) {
                player.location.y = 0;
            }
        } else if (player.action.down) {
            player.location.y += 1;
            if (player.location.y > SCREEN_HEIGHT - PLAYER_HEIGHT) {
                player.location.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
            }
        }

        if (player.action.left) {
            player.location.x -= 1;
            if (player.location.x < 0) {
                player.location.x = 0;
            }
        } else if (player.action.right) {
            player.location.x += 1;
            if (player.location.x > SCREEN_WIDTH - PLAYER_WIDTH) {
                player.location.x = SCREEN_WIDTH - PLAYER_WIDTH;
            }
        }

        // First clear the renderer
        SDL_RenderClear(ren);
        // Draw the texture
        SDL_RenderCopy(ren, tex, &player.sprite, &player.location);
        // Update the screen
        SDL_RenderPresent(ren);

        while (SDL_GetTicks() < start_tick + frame_time) {
            SDL_Delay(1);
        }
        frame_count += 1;
        if (SDL_GetTicks() > second) {
            second += 1000;
            SDL_Log("Frames: %d\n", frame_count);
            frame_count = 0;
        }
        stop_tick = SDL_GetTicks();
        if (stop_tick - start_tick > frame_time + 2) {
            SDL_Log("Big frame size %d\n", stop_tick - start_tick);
        }
    }
    return 0;
}