#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
	
	if(argc == 4)
	{
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("Failed to initialize the SDL2 library\n");
			return -1;
		}
		SDL_DisplayMode mode;
		SDL_GetDesktopDisplayMode(0, &mode);
		int h = atoi(argv[1]);
		int w = atoi(argv[2]);
		if(h <= mode.h && w <= mode.w)
		{
			//cette partie devra potentiellement être transférée dans une autre fonction.
			if(strcmp(argv[3], "Mandelbrot") == 0)
			{
				SDL_Window *window = SDL_CreateWindow("MANNY!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, h, w, 0);
				if(!window)
				{
					printf("Failed to create window\n");
					return -1;
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
						printf("Failed to create window\n");
						return -1;
					}
					SDL_UpdateWindowSurface(window);
					SDL_Delay(5000);
					}
				else
				{
					printf("Les deux noms de fractales valides sont 'Mandelbrot' et 'Julia'\n");
					return 1;
				}
			}
			//fin de la partie potentiellement transférée
		}
		else
		{
			printf("Dimensions incorrectes!\n");
			return 1;
		}
	}
	else
	{
		printf("Mauvais nombre d'arguments!\n");
		return 1;
	}
	return 0;
}
