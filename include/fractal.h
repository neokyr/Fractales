//
// Created by pad on 1/10/23.
//

#ifndef FRACTALES_FRACTAL_H
#define FRACTALES_FRACTAL_H

#include <SDL2/SDL.h>

typedef struct s_complex {
    double real;
    double img;
} t_complex;

typedef struct s_range {
    float minX;
    float maxX;
    float minY;
    float maxY;
    int maxIter;
    float maxDeviation;
} t_range;

typedef struct s_color_palette {
    Uint32 *palette;
    unsigned short number_of_color:7;
    bool linear_interpolation:1;
} t_colors;

int fractals(SDL_Window *pWindow, t_range range, t_colors colors, int z_index);

t_complex square(t_complex nb);

#endif //FRACTALES_FRACTAL_H
