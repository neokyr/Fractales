#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "error.h"
#include "screen_handling.h"

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
				SDL_UpdateWindowSurface(window);
				bool keep_window_open = true;
				while(keep_window_open)
				{
					SDL_Event e;
					while(SDL_PollEvent(&e) > 0)
					{
						handleEvents(&e, &keep_window_open);
					}
				}
				//à "remplacer" par l'event handler*/
				SDL_Delay(100);
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
