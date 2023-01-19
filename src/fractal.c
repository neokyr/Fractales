//
// Created by pad on 1/10/23.
//
#include "screen_handling.h"
#include "error.h"
#include "open_cl.h"


uint32_t get_linear_interpolation(int max, t_colors colors, int nb_iter_dev) {


    double offset_norm = (nb_iter_dev % (max / (colors.number_of_color - 1)));
    //	printf("%d %% %d / %d = %f\n",nb_iter_dev, range.maxIter, (colors.number_of_color-1), offset_norm);
    offset_norm = offset_norm / (((double) max / (double) (colors.number_of_color - 1)));
    // printf("%f\n", offset_norm);
    int index_base_color = ((nb_iter_dev * (colors.number_of_color-1)) / max);
    uint32_t color_1 = colors.palette[index_base_color];


    if (nb_iter_dev == max) {
        return (color_1);
    } else {


        uint32_t color_2 = colors.palette[index_base_color + 1];
        uint32_t color_3;
        for (int i = 0; i < 4; i++) {
            /*double a = ((char*) &color_1)[i] * (1 - offset_norm);
            double b = ((char*) &color_2)[i] * offset_norm;
            ((char*) &color_3)[i] = floor(a+b);*/
            double y0 = ((char *) &color_1)[i];
            double y1 = ((char *) &color_2)[i];
            double x0 = index_base_color;
            double x1 = index_base_color + 1;
            double xp = (double) nb_iter_dev * (double) (colors.number_of_color - 1) / (double) max;

            double yp = y0 + ((y1 - y0) / (x1 - x0)) * (xp - x0);

            ((char *) &color_3)[i] = yp;
        }
        return (color_3);
    }
}

uint32_t solve(cl_int num, int max, t_colors colors) {


    if (!colors.linear_interpolation) {
        return colors.palette[num * (colors.number_of_color - 1)  /max];
    } else {
        return get_linear_interpolation(max, colors, num);
    }
}

int fractals(SDL_Window *pWindow, t_range range, t_colors colors, t_opencl *pCl) {
    SDL_Surface *window_surface = SDL_GetWindowSurface(pWindow);
    if (window_surface == NULL) return print_error(ERROR_SDL_NO_SURFACE);

    int err = findDeviation(pCl, &range, *window_surface);
    if(err != 0 ) return err;

    for (int i = 0; i < window_surface->w; ++i) {
        for (int j = 0; j < window_surface->h; ++j) {
            put_pixel(window_surface->pixels, i, j, solve(pCl->results[j* window_surface->w + i], range.maxIter, colors), window_surface->w);
        }
    }

    return 0;
}

t_complex square(t_complex nb) {
    t_complex result;

    result.real = nb.real * nb.real - (nb.img * nb.img);
    result.img = 2 * nb.real * nb.img;

    return result;
}
