#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_SIZE 16

typedef struct Window {
    unsigned int mode;
    unsigned int pixelSize;
    int w;
    int h;
    int q1w; // left quarter
    int q3w; // right quarter
    int q1h; // end of top quarter 
    int q3h; // begin of bottom quarter
    SDL_Window *ptr;
    SDL_Renderer *renderer;
} Window;

void initWindow(Window *win);
void windowSizeChanged(Window *win, int w, int h);

#endif