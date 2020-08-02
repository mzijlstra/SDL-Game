#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>

#define PLAYER_WIDTH 24
#define PLAYER_HEIGHT 16
#define PLAYER_FLAME_FRAMES 2

typedef struct PlayerAction {
    SDL_bool up;
    SDL_bool down;
    SDL_bool left;
    SDL_bool right;
    SDL_bool boost;
    SDL_bool fire;
} PlayerAction;

typedef struct PlayerLocation {
    double x;
    double y;
    unsigned int max_w;
    unsigned int max_h;
} PlayerLocation;

typedef struct PlayerAcceleration {
    double up;
    double down;
    double left;
    double right;
    double boost;
} PlayerAcceleration;

typedef struct PlayerVelocity {
    double x;
    double y;
} PlayerVelocity;

typedef struct PlayerTimer {
    uint8_t up_count;
    uint8_t down_count;
} PlayerTimer;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerTimer timer;
    SDL_Rect sprite;
    SDL_Rect screen;
} Player;

int main() {
    unsigned int win_mode = 0;
    unsigned int pixel_size = 1;
    unsigned int screen_width = 480;
    unsigned int screen_height = 270;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, screen_width,
                                       screen_height, SDL_WINDOW_SHOWN);
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

    SDL_DisableScreenSaver();

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
    player.action.boost = SDL_FALSE;
    player.location.x = 100.0;
    player.location.y = 100.0;
    player.location.max_w = screen_width - PLAYER_WIDTH;
    player.location.max_h = screen_height - PLAYER_HEIGHT;
    player.acceleration.up    = 0.7;
    player.acceleration.down  = 0.7;
    player.acceleration.right = 0.7;
    player.acceleration.left  = 0.7;
    player.acceleration.boost = 1.3;
    player.velocity.x = 0;
    player.velocity.y = 0;
    player.timer.up_count = 0;
    player.timer.down_count = 0;
    player.sprite.x = 24;
    player.sprite.y = 32;
    player.sprite.w = PLAYER_WIDTH;
    player.sprite.h = PLAYER_HEIGHT;
    player.screen.x = player.location.x * pixel_size;
    player.screen.y = player.location.y * pixel_size;
    player.screen.w = PLAYER_WIDTH * pixel_size;
    player.screen.h = PLAYER_HEIGHT * pixel_size;

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
            case SDL_WINDOWEVENT:
                SDL_GetWindowSize(win, &screen_width, &screen_height);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = SDL_FALSE;
                    break;
                case SDLK_f:
                    if (win_mode == 0) {
                        SDL_SetWindowFullscreen(win,
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
                        win_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
                        pixel_size = 4;
                        player.screen.x *= pixel_size;
                        player.screen.y *= pixel_size;
                        player.screen.w *= pixel_size;
                        player.screen.h *= pixel_size;
                    } else if (win_mode == SDL_WINDOW_FULLSCREEN_DESKTOP) {
                        SDL_SetWindowFullscreen(win, 0);
                        win_mode = 0;
                        pixel_size = 1;
                        player.screen.x /= 4;
                        player.screen.y /= 4;
                        player.screen.w /= 4;
                        player.screen.h /= 4;
                        ;
                    }
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
                case SDLK_TAB:
                    player.action.boost = SDL_TRUE;
                    player.sprite.x = 0;
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
                    player.timer.up_count = 0;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    player.action.right = SDL_FALSE;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    player.action.down = SDL_FALSE;
                    player.timer.down_count = 0;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    player.action.left = SDL_FALSE;
                    break;
                case SDLK_TAB:
                    player.action.boost = SDL_FALSE;
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
            // move ship
            player.velocity.y -= player.acceleration.up;
            // tilt ship
            player.timer.up_count += 1;
            if (player.action.boost && player.action.right &&
                player.timer.up_count > 15) {
                player.sprite.y = 0;
            } else {
                player.sprite.y = 16;
            }
        } else if (player.action.down) {
            // move ship
            player.velocity.y += player.acceleration.down;
            // tilt ship
            player.timer.down_count += 1;
            if (player.action.boost && player.action.right &&
                player.timer.down_count > 15) {
                player.sprite.y = 64;
            } else {
                player.sprite.y = 48;
            }
        } else {
            player.sprite.y = 32;
        }


        if (player.action.left) {
            // move ship
            player.velocity.x -= player.acceleration.left;
            // set ship flame
            player.sprite.x = 72;
        } else if (player.action.right) {
            // move ship
            if (player.action.boost) {
                player.velocity.x += player.acceleration.boost;
            } else {
                player.velocity.x += player.acceleration.right;
            }
            
            // set ship flame
            if (player.action.boost) {
                if (frame_count % 5 == 0) {
                    player.sprite.x = (player.sprite.x + PLAYER_WIDTH) % 48;
                }
            } else {
                player.sprite.x = 48;
            }
        } else {
            player.sprite.x = 72;
        }


        player.velocity.x = player.velocity.x * (1.0 - 0.2); // 0.2 is drag
        player.velocity.y = player.velocity.y * (1.0 - 0.2);
        if (fabs(player.velocity.x) < 0.1) {
            player.velocity.x = 0;
        }
        if (fabs(player.velocity.y) < 0.1) {
            player.velocity.y = 0;
        }

        player.location.y += player.velocity.y;
        if (player.location.y < 0) {
            player.location.y = 0;
        } else if (player.location.y > player.location.max_h) {
            player.location.y = player.location.max_h;
        }
        player.screen.y = ((int)player.location.y) * pixel_size;

        player.location.x += player.velocity.x;
        if (player.location.x < 0) {
            player.location.x = 0;
        } else if (player.location.x > player.location.max_w) {
            player.location.x = player.location.max_w;
        }
        player.screen.x = ((int)player.location.x) * pixel_size;


        // First clear the renderer
        SDL_RenderClear(ren);
        // Draw the texture
        SDL_RenderCopy(ren, tex, &player.sprite, &player.screen);

        // Update the screen
        SDL_RenderPresent(ren);

        while (SDL_GetTicks() < start_tick + frame_time) {
            SDL_Delay(1);
        }
        frame_count += 1;
        if (SDL_GetTicks() > second) {
            second += 1000;
            // SDL_Log("Frames: %d\n", frame_count);
            frame_count = 0;
        }
        stop_tick = SDL_GetTicks();
        if (stop_tick - start_tick > frame_time + 2) {
            SDL_Log("Big frame size %d\n", stop_tick - start_tick);
        }
    }
    return 0;
}