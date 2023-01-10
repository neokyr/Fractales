#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "error.c"

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
		if(strcmp(endPtrH, "") != 0 || strcmp(endPtrW, "") != 0)
		{
			return print_error(ERROR_NAN);
		}
		if(h <= mode.h && w <= mode.w)
		{
			if(strcmp(argv[3], "Mandelbrot") == 0)
			{
				SDL_Window *window = SDL_CreateWindow("MANNY!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, h, w, 0);
				if(!window)
				{
					return print_error(ERROR_SDL_WINDOW_NULL);
				}
				SDL_UpdateWindowSurface(window);
				SDL_Delay(5000);
			}
			else
			{
				if(strcmp(argv[3], "Julia") == 0)
				{
					SDL_Window *window = SDL_CreateWindow("JUJU!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, h, w, 0);
					if(!window)
					{
						return print_error(ERROR_SDL_WINDOW_NULL);
					}
					SDL_UpdateWindowSurface(window);
					SDL_Delay(5000);
					}
				else
				{
					return print_error(ERROR_INCORRECT_TYPE);
				}
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
