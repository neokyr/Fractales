//
// Created by pad on 1/10/23.
//

#include "../include/fractal.h"
#include "../include/screen_handling.h"
#include "../include/error.h"
#include <stdbool.h>
#include <stdio.h>

uint32_t solve(t_complex c, t_range range, t_colors colors, t_complex z0);

uint32_t get_linear_interpolation(t_range range, t_colors colors, int nb_iter_dev){
	double offset_norm = (nb_iter_dev % (range.maxIter / (colors.number_of_color -1 )));
	//	printf("%d %% %d / %d = %f\n",nb_iter_dev, range.maxIter, (colors.number_of_color-1), offset_norm);
	offset_norm = offset_norm / ((double)range.maxIter / (double)(colors.number_of_color -1));
	// printf("%f\n", offset_norm);
	int index_base_color = (nb_iter_dev / (range.maxIter / (colors.number_of_color -1 )));
	uint32_t color_1 = colors.palette[index_base_color];
	

	if ( nb_iter_dev == range.maxIter ){
		return(color_1);
	}	

	else {
		uint32_t color_2 = colors.palette[index_base_color + 1];
		for (int i = 0; i < 4; i++){
			((char*) &color_1)[i] *= (1 - offset_norm);
			((char*) &color_2)[i] *= offset_norm;
		}
		return(color_1 + color_2);
	}
}

uint32_t solve(t_complex c, t_range range, t_colors colors, t_complex z0) {
    int i;
    t_complex sqZ, z = z0;
    for (i = 0; i < range.maxIter; ++i) {
        sqZ = square(z);
        //Zn+1 = Zn^2 + c
        z.real = sqZ.real + c.real;
        z.img = sqZ.img + c.img;

        if(fabs(z.real + z.img) > range.maxDeviation) break;
    }

    if(!colors.linear_interpolation) {
        return colors.palette[(i*colors.number_of_color)/(range.maxIter+1)];
    } else {
        return get_linear_interpolation(range, colors, i);
    }
}

int fractals(SDL_Window *pWindow, t_range range, t_colors colors) {
    SDL_Surface* window_surface = SDL_GetWindowSurface(pWindow);
    if(window_surface == NULL) return print_error(ERROR_SDL_NO_SURFACE);
    double yIncrement = (range.maxY - range.minY) / (double) window_surface->h;
    double xIncrement = (range.maxX - range.minX) / (double) window_surface->w;


    double y = range.minY;
    for (int i = 0  ; i < window_surface->h; ++i, y+=yIncrement) {
        double x = range.minX;
        for (int j = 0; j < window_surface->w; ++j, x+=xIncrement) {
            t_complex curr;
            curr.real=x;
            curr.img =y;

            // Formula is Zn+1 = Zn^2 + c
            if(range.isMandelbrot) // Mandelbrot has a fixed z_0 and changing c
                put_pixel(window_surface->pixels, j, i, solve(curr, range, colors, range.unchanging), window_surface->w);
            else // Julia has a fixed c and changing z_0
                put_pixel(window_surface->pixels, j, i, solve(range.unchanging, range, colors, curr), window_surface->w);
        }
    }

    return 0;
}

t_complex square(t_complex nb) {
    t_complex result;

    result.real = nb.real * nb.real - nb.img * nb.img;
    result.img = 2 * nb.real * nb.img;

    return result;
}
