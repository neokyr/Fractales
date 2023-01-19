#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "screen_handling.h"
#include "open_cl.h"

int main(int argc, char **argv)
{
	if(argc == 4)
	{
#ifdef __linux__
        setenv("SDL_VIDEODRIVER", "x11", 1);
#endif

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
			if(strcmp(argv[3], "Mandelbrot") == 0 || strcmp(argv[3], "Julia") == 0)
			{
				SDL_Window *window = SDL_CreateWindow("Fractales", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
				if(!window)
				{
					return print_error(ERROR_SDL_WINDOW_NULL);
				}
				bool keep_window_open = true;
				SDL_Event e;
				t_range range;
				range.isMandelbrot = strcmp(argv[3], "Mandelbrot") == 0;
				range.maxDeviation = 4;
				range.maxIter = 200;
                t_complex unchanging;
                if(range.isMandelbrot) {
                    range.maxX = 1;
                    range.maxY = 1.5;
                    range.minX = -2.5;
                    range.minY = -1.5;
                    unchanging.real = 0;
                    unchanging.img = 0;
                } else {
                    range.maxX = 2;
                    range.maxY = 2;
                    range.minX = -2;
                    range.minY = -2;
                    unchanging.real = -0.4;
                    unchanging.img = 0.6;
                }

				range.unchanging = unchanging;
                t_colors palettes[3];

				Uint32 f_palette[12] = {0x0000ff00, 0x0080ff00, 0xff008000, 0xff00ff00, 0x8000ff00, 0x00ffff00, 0x00ff8000, 0x00ff0000, 0x80ff0000, 0xffff0000, 0xff800000, 0x00000000 };
                palettes[0].palette = (Uint32*) &f_palette;
                palettes[0].linear_interpolation = true;
                palettes[0].number_of_color = 12;

                Uint32 s_palette[6] ={ 0x00940A00, 0x00AD0C00, 0x00C70D00, 0x00E00F00, 0x00FA1000,0x00000000, };
                palettes[1].palette = (Uint32*) &s_palette;
                palettes[1].linear_interpolation = true;
                palettes[1].number_of_color = 6;

                Uint32 t_palette[3] ={  0xFFFFFF00, 0xDE860400, 0xFF280000 };
                palettes[2].palette = (Uint32*) &t_palette;
                palettes[2].linear_interpolation = false;
                palettes[2].number_of_color = 3;

                int current_palette = 0;
				bool isVariationActive = false;
				bool isFullScreen = false;

                t_opencl opencl;
                int err = clInit(&opencl);
                if(err != 0) return err;

				while(keep_window_open)
				{
                    err = fractals(window, range, palettes[current_palette], &opencl);
					if (err != 0) {
						SDL_Quit();
						return err;
					}

					SDL_UpdateWindowSurface(window);
                    handleEvents(&e, &keep_window_open, &range, window, &isVariationActive, &palettes[current_palette], &current_palette, &isFullScreen);
					SDL_Delay(16);
				}
			}
			else
			{
				return print_error(ERROR_INCORRECT_TYPE, argv[3]);
			}
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
