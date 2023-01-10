//
// Created by pad on 1/10/23.
//

#include "../include/fractal.h"
#include "../include/screen_handling.h"
#include "../include/error.h"
#include <stdbool.h>


int fractals(SDL_Window *pWindow, t_range range, t_colors colors) {
    SDL_Surface* window_surface = SDL_GetWindowSurface(pWindow);
    if(window_surface == NULL) return print_error(ERROR_SDL_NO_SURFACE);
    double yIncrement = (range.maxY - range.minY) / (double) window_surface->h;
    double xIncrement = (range.maxX - range.minX) / (double) window_surface->w;

    double y = range.minY;
    for (int i = 0  ; i < window_surface->h; ++i, y+=yIncrement) {
        double x = range.minX
        for (int j = 0, x = range.minX; j < window_surface->w; ++j, x+=xIncrement) {
            t_complex curr;
            curr.real=x;
            curr.img =y;

            // Formula is Zn+1 = Zn^2 + c
            if(range.isMandelbrot) // Mandelbrot has a fixed z_0 and changing c
                put_pixel(window_surface->pixels, j, i, solve(curr, range, colors, range.unchanging));
            else // Julia has a fixed c and changing z_0
                put_pixel(window_surface->pixels, j, i, solve(range.unchanging, range, colors, curr));
        }
    }

    return 0;
}

uint32_t solve(t_complex c, t_range range, t_colors colors, t_complex z0) {
    int i;
    t_complex z = z0;
    for (i = 0; i < range.maxIter; ++i) {
        z.real = square(z).real + c.real;
        z.img = square(z).img + c.img;

        if(fabs(z.real + z.img) > range.maxDeviation) break;
    }

    if(!colors.linear_interpolation) {
        return colors.palette[i/(range.maxIter/colors.number_of_color)];
    } else {
        //TODO linear_interpolation
        return 0x00000000;
    }
}

t_complex square(t_complex nb) {
    t_complex result;

    result.real = nb.real * nb.real - nb.img * nb.img;
    result.img = 2 * nb.real * nb.img;

    return result;
}