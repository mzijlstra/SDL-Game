#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Window {
    unsigned int mode;
    unsigned int pixel_size;
    int width;
    int height;
    SDL_Window *ptr;
    SDL_Renderer *renderer;
} Window;

void initWindow(Window *win);
SDL_Texture *loadTexture(char *asset, SDL_Renderer *ren);


#endif