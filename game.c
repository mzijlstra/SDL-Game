#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16

typedef struct Window {
    unsigned int mode;
    unsigned int pixel_size;
    unsigned int width;
    unsigned int height;
    SDL_Window *ptr;
    SDL_Renderer *renderer;
} Window;

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

typedef struct PlayerTexture {
    SDL_Texture *ship;
    SDL_Texture *flame;
} PlayerTexture;

typedef struct Player {
    PlayerAction action;
    PlayerLocation location;
    PlayerAcceleration acceleration;
    PlayerVelocity velocity;
    PlayerTimer timer;
    PlayerTexture texture;
    SDL_Rect ship;
    SDL_Rect shipDest;
    SDL_Rect flame;
    SDL_Rect flameDest;
} Player;

SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren) {
    SDL_Texture *tex = NULL;
    SDL_Surface *img = IMG_Load(asset);
    SDL_bool failed = SDL_FALSE;
    if (img == NULL) {
        SDL_Log("IMG_Load Error: %s\n", SDL_GetError());
        failed = SDL_TRUE;
    } else {
        tex = SDL_CreateTextureFromSurface(ren, img);
        if (tex == NULL) {
            SDL_Log("SDL_CreateTexture: %s\n", SDL_GetError());
            failed = SDL_TRUE;
        }
    }
    SDL_FreeSurface(img);

    // On failure return a placeholder image!
    if (failed) {
        SDL_Log("Creating Placeholder Texture");
        tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 1024, 1024);
        SDL_SetRenderTarget(ren, tex);
        SDL_SetRenderDrawColor(ren, 0x00, 0xff, 0xff, 0xff);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0xff, 0xff);
        SDL_Rect r;
        r.w = 4;
        r.h = 4;
        for (int y = 4; y < 1024; y += 8) {
            for (int x = 4; x < 1024; x += 8) {
                r.x = x;
                r.y = y;
                SDL_RenderFillRect(ren, &r);
            }
        }
        SDL_SetRenderTarget(ren, NULL);
    }

    return tex;
}

void initWindow(Window *window) {
    window->mode = 0;
    window->pixel_size = 1;
    // TODO set these to 1/4 of desktop resolution
    window->width = 480;
    window->height = 270;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    window->ptr = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, window->width,
                                   window->height, SDL_WINDOW_SHOWN);
    if (window->ptr == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }

    window->renderer = SDL_CreateRenderer(
        window->ptr, -1,
        SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC
    if (window->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_DisableScreenSaver();
}

void initPlayer(Player *player, Window *window) {
    player->action.up = SDL_FALSE;
    player->action.down = SDL_FALSE;
    player->action.left = SDL_FALSE;
    player->action.right = SDL_FALSE;
    player->action.boost = SDL_FALSE;
    player->location.x = 100.0;
    player->location.y = 100.0;
    player->location.max_w = window->width - PLAYER_WIDTH;
    player->location.max_h = window->height - PLAYER_HEIGHT;
    player->acceleration.up = 0.7;
    player->acceleration.down = 0.7;
    player->acceleration.right = 0.7;
    player->acceleration.left = 0.7;
    player->acceleration.boost = 1.3;
    player->velocity.x = 0;
    player->velocity.y = 0;
    player->timer.up_count = 0;
    player->timer.down_count = 0;
    player->texture.ship = loadTexture("assets/ship.png", window->renderer);
    player->texture.flame = loadTexture("assets/flame.png", window->renderer);
    player->ship.x = 0;
    player->ship.y = 32;
    player->ship.w = PLAYER_WIDTH;
    player->ship.h = PLAYER_HEIGHT;
    player->shipDest.x = player->location.x * window->pixel_size;
    player->shipDest.y = player->location.y * window->pixel_size;
    player->shipDest.w = PLAYER_WIDTH * window->pixel_size;
    player->shipDest.h = PLAYER_HEIGHT * window->pixel_size;
    player->flame.x = 0;
    player->flame.y = 0;
    player->flame.w = 16;
    player->flame.h = 16;
    player->flameDest.x = (player->location.x - 8) * window->pixel_size;
    player->flameDest.y = player->location.y * window->pixel_size;
    player->flameDest.w = PLAYER_WIDTH * window->pixel_size;
    player->flameDest.h = PLAYER_HEIGHT * window->pixel_size;
}

int main() {
    Window window;
    initWindow(&window);

    Player player;
    initPlayer(&player, &window);

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
                SDL_GetWindowSize(window.ptr, &window.width, &window.height);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = SDL_FALSE;
                    break;
                case SDLK_f:
                    if (window.mode == 0) {
                        SDL_SetWindowFullscreen(window.ptr,
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
                        window.mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
                        window.pixel_size = 4;
                        player.shipDest.x *= window.pixel_size;
                        player.shipDest.y *= window.pixel_size;
                        player.shipDest.w *= window.pixel_size;
                        player.shipDest.h *= window.pixel_size;
                        player.flameDest.x *= window.pixel_size;
                        player.flameDest.y *= window.pixel_size;
                        player.flameDest.w *= window.pixel_size;
                        player.flameDest.h *= window.pixel_size;
                    } else if (window.mode == SDL_WINDOW_FULLSCREEN_DESKTOP) {
                        SDL_SetWindowFullscreen(window.ptr, 0);
                        window.mode = 0;
                        window.pixel_size = 1;
                        player.shipDest.x /= 4;
                        player.shipDest.y /= 4;
                        player.shipDest.w /= 4;
                        player.shipDest.h /= 4;
                        player.flameDest.x /= 4;
                        player.flameDest.y /= 4;
                        player.flameDest.w /= 4;
                        player.flameDest.h /= 4;
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

        // apply player actions
        if (player.action.up) {
            // move ship
            player.velocity.y -= player.acceleration.up;
            // tilt ship
            player.timer.up_count += 1;
            if (player.action.boost && player.action.right &&
                player.timer.up_count > 15) {
                player.ship.y = 0;
            } else {
                player.ship.y = 16;
            }
        } else if (player.action.down) {
            // move ship
            player.velocity.y += player.acceleration.down;
            // tilt ship
            player.timer.down_count += 1;
            if (player.action.boost && player.action.right &&
                player.timer.down_count > 15) {
                player.ship.y = 64;
            } else {
                player.ship.y = 48;
            }
        } else {
            player.ship.y = 32;
        }

        if (player.action.left) {
            // move ship
            player.velocity.x -= player.acceleration.left;
            // set ship flame
            player.flame.x = 48;
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
                    player.flame.x = (player.flame.x + PLAYER_WIDTH) % 32;
                }
            } else {
                player.flame.x = 32;
            }
        } else {
            player.flame.x = 48;
        }

        // update velocity
        player.velocity.x = player.velocity.x * (1.0 - 0.2); // 0.2 is drag
        player.velocity.y = player.velocity.y * (1.0 - 0.2);
        if (fabs(player.velocity.x) < 0.1) {
            player.velocity.x = 0;
        }
        if (fabs(player.velocity.y) < 0.1) {
            player.velocity.y = 0;
        }

        // calculate on screen locations
        player.location.y += player.velocity.y;
        if (player.location.y < 0) {
            player.location.y = 0;
        } else if (player.location.y > player.location.max_h) {
            player.location.y = player.location.max_h;
        }
        player.shipDest.y = ((int)player.location.y) * window.pixel_size;
        player.flameDest.y = player.shipDest.y;

        player.location.x += player.velocity.x;
        if (player.location.x < 0) {
            player.location.x = 0;
        } else if (player.location.x > player.location.max_w) {
            player.location.x = player.location.max_w;
        }
        player.shipDest.x = ((int)player.location.x) * window.pixel_size;
        player.flameDest.x = ((int)player.location.x - 8) * window.pixel_size;

        // First clear the renderer
        SDL_SetRenderDrawColor(window.renderer, 200, 200, 200, 255);
        SDL_RenderClear(window.renderer);
        // Draw the textures
        SDL_RenderCopy(window.renderer, player.texture.ship, &player.ship,
                       &player.shipDest);
        SDL_RenderCopy(window.renderer, player.texture.flame, &player.flame,
                       &player.flameDest);

        // Update the screen
        SDL_RenderPresent(window.renderer);

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