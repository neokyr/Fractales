//
// Created by pad on 1/5/23.
//

#include "../include/error.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "../include/screen_handling.h"

bool isLittleEndian() {
    int n = 1;
    return *(char*)&n == 1; //if the first byte of n is equal to 1 then the system is littleEndian
}

void put_pixel(char* buffer, int x, int y, unsigned int color, int w) {
    char r, g, b, a;
    if (isLittleEndian()) {
        r = ((char *) &color)[3];
        g = ((char *) &color)[2];
        b = ((char *) &color)[1];
        a = ((char *) &color)[0];

    } else {
        r = ((char *) &color)[0];
        g = ((char *) &color)[1];
        b = ((char *) &color)[2];
        a = ((char *) &color)[3];
    }

    int start = (y * w * 4) + x * 4;

    buffer[start] = b;
    buffer[start+1] = g;
    buffer[start+2] = r;
    buffer[start+3] = a;
}

int background(SDL_Window* pWindow, unsigned int color) {
    SDL_Surface* surface = SDL_GetWindowSurface(pWindow);
    if(surface == NULL) return print_error(ERROR_SDL_NO_SURFACE);
    int h = surface->h;
    int w = surface->w;

    return print_square(pWindow, color, 0, 0, w, h);
}

int print_square(SDL_Window* pWindow, unsigned int color, int x, int y, int w, int h) {
    SDL_Surface* surface = SDL_GetWindowSurface(pWindow);
    if(surface == NULL) return print_error(ERROR_SDL_NO_SURFACE);
    char* pixelBuffer = surface->pixels;
    int mh = surface->h;
    int mw = surface->w;


    for (int i = x; i < x+w && i < mw; ++i) {
        for (int j = y; j < y+h && j < mh; ++j) {
            put_pixel(pixelBuffer, i, j, color, mw);
        }
    }

    /*if(SDL_UpdateWindowSurface(pWindow)) return print_error(ERROR_SDL_WINDOW_UPDATE);*/

    return 0;
}

void handleEvents(SDL_Event *event, bool* gameRunning) {
    SDL_KeyCode keyPressed;
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_KEYDOWN:
                keyPressed = event->key.keysym.sym;
                if (keyPressed == QUIT_KEY)
                {
                    *gameRunning = false;
                    break;
                }

                break;
            case SDL_QUIT: /* if mouse click to close window */
            {
                *gameRunning = false;
                break;
            }
            case SDL_KEYUP: {
                break;
            }
        }
    }
}
