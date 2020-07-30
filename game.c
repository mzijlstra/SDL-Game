#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Window *win =
        SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 480, 270, SDL_WINDOW_SHOWN);
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

    SDL_Rect srect;
    srect.x = 24;
    srect.y = 32;
    srect.w = 24;
    srect.h = 16;
    SDL_Rect drect;
    drect.x = 100;
    drect.y = 100;
    drect.w = 24;
    drect.h = 16;
    
    SDL_bool player_up = SDL_FALSE;
    SDL_bool player_down = SDL_FALSE;
    SDL_bool player_left = SDL_FALSE;
    SDL_bool player_right = SDL_FALSE;

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
                    player_up = SDL_TRUE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    player_right = SDL_TRUE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    player_down = SDL_TRUE;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    player_left = SDL_TRUE;
                    break;
                default:
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    player_up = SDL_FALSE;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    player_right = SDL_FALSE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    player_down = SDL_FALSE;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    player_left = SDL_FALSE;
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

        if (player_up) {
            drect.y -= 1;
        } else if (player_down) {
            drect.y += 1;
        }

        if (player_left) {
            drect.x -= 1;
        } else if (player_right) {
            drect.x += 1;
        }

        // First clear the renderer
        SDL_RenderClear(ren);
        // Draw the texture
        SDL_RenderCopy(ren, tex, &srect, &drect);
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