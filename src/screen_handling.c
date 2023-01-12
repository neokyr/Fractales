//
// Created by pad on 1/5/23.
//

#include "../include/error.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "../include/screen_handling.h"
#include "fractal.h"

#define movePercent 0.02

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

void handleEvents(SDL_Event *event, bool* gameRunning, t_range *range, SDL_Window *window, bool* isVariationActive, t_colors *colors) {
	
	float xRange = range->maxX - range->minX;
	float yRange = range->maxY - range->minY;
    SDL_KeyCode keyPressed;
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_KEYDOWN:
                keyPressed = event->key.keysym.sym;
                switch (keyPressed) {
                	case QUIT_KEY:
						*gameRunning = false;
						break;
					//switching between the fractals
                	case SWITCH_KEY:
                		range->isMandelbrot = !range->isMandelbrot;
            			range->maxX = 1;
						range->maxY = 1.5;
						range->minX = -2.5;
						range->minY = -1.5;
     		   			if(range->isMandelbrot)
            			{
            				//deactivating variation when going back to Mandelbrot
            				*isVariationActive = false;
            				t_complex unchanging;
							unchanging.real = 0;
							unchanging.img = 0;
            				range->unchanging = unchanging;
            			}
                		break;
            		//moving the fractal
            		//(-1.5, -2.5) is in the top left, (1, 1.5) is in the bottom right 
                	case UP_MOVE_KEY:
                		range->maxY -= movePercent * yRange;
                		range->minY -= movePercent * yRange;
                		break;
                	case DOWN_MOVE_KEY:
                		range->maxY += movePercent * yRange;
                		range->minY += movePercent * yRange;
                		break;
                	case LEFT_MOVE_KEY:
                		range->maxX -= movePercent * xRange;
                		range->minX -= movePercent * xRange;
                		break;
                	case RIGHT_MOVE_KEY:
                		range->maxX += movePercent * xRange;
                		range->minX += movePercent * xRange;
                		break;
            		//switch interpolation on and off
            		case INTERPOLATION_KEY:
            			colors->linear_interpolation = !colors->linear_interpolation;
            			break;
            		case CHANGE_COLOR_KEY:
            			//insérer le code pour changer la couleur
            			break;
            		default :
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
            //switching the variations on Julia
            case SDL_MOUSEBUTTONDOWN: {
            	if(event->button.button == CLICK){
            		if(!range->isMandelbrot)
            		{
            			*isVariationActive = !(*isVariationActive);
            		}
            	}
            	break;
            }
            case SDL_MOUSEBUTTONUP: {
            	break;
            }
            //changing the variation on Julia
            case SDL_MOUSEMOTION: {
            	if(*isVariationActive)
            	{
            		t_complex unchanging;
        			int x =0;
        			int y=0;
        			SDL_Surface *surf = SDL_GetWindowSurface(window);
					SDL_GetMouseState(&x, &y);
					unchanging.real = ((((float)x) / surf->w) * 4)-2;
					unchanging.img = ((((float)y) / surf->h) * 4)-2;
					range->unchanging = unchanging;
            	}
            	break;
            }
            default:
            	break;
        }
    }
}
