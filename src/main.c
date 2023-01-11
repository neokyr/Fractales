#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "error.h"
#include "screen_handling.h"
#include "fractal.h"

int main(int argc, char **argv)
{
	if(argc == 4)
	{
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			return print_error(ERROR_SDL_INIT);
		}
		
		SDL_DisplayMode mode;
		SDL_GetDesktopDisplayMode(0, &mode);
		char * endPtrH;
		char * endPtrW;
		int h = (int) strtol(argv[1], &endPtrH, 10);
		int w = (int) strtol(argv[2], &endPtrW, 10);
		if(strcmp(endPtrH, "") != 0)
		{
			return print_error(ERROR_NAN, argv[1]);
		}
		if(strcmp(endPtrW, "") != 0)
		{
			return print_error(ERROR_NAN, argv[2]);
		}
		if(h <= mode.h && w <= mode.w)
		{
			//cette partie reste à modifier
			if(strcmp(argv[3], "Mandelbrot") == 0 || strcmp(argv[3], "Julia") == 0)
			{
				SDL_Window *window = SDL_CreateWindow("MANNY!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
				if(!window)
				{
					return print_error(ERROR_SDL_WINDOW_NULL);
				}
				bool keep_window_open = true;
				SDL_Event e;
				while(keep_window_open)
				{
          t_range range;
                range.isMandelbrot = strcmp(argv[3], "Mandelbrot") == 0;
                range.maxDeviation = 16;
                range.maxIter = 100;
                range.maxX = 1;
                range.maxY = 1.5;
                range.minX = -2.5;
                range.minY = -1.5;
                t_complex unchanging;
                unchanging.real = 0;
                unchanging.img = 0;
                range.unchanging = unchanging;

                t_colors colors;
                Uint32 palette[6] = { 0x00459E00, 0x43900400, 0xF5DD1100, 0xDE860400, 0xFF280000, 0x00000000 };
                colors.palette = (Uint32*) &palette;
                colors.linear_interpolation = false;
                colors.number_of_color = 6;

                int err = fractals(window, range, colors);
                if (err != 0) {
                    SDL_Quit();
                    return err;
                }

                SDL_UpdateWindowSurface(window);
					handleEvents(&e, &keep_window_open);
					SDL_Delay(17);
				}
			}
			else
			{
				return print_error(ERROR_INCORRECT_TYPE, argv[3]);
			}
			//fin de la partie potentiellement transférée
		}
		else
		{
			return print_error(ERROR_OOB_DIMENSIONS);
		}
	}
	else
	{
		return print_error(ERROR_ARG_NBR);
	}
	return 0;
}
