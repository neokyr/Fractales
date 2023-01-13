//
// Created by pad on 1/10/23.
//

#ifndef FRACTALES_FRACTAL_H
#define FRACTALES_FRACTAL_H

#include <SDL2/SDL.h>
#include <stdbool.h>

/**
 * Complex number with an imaginary and real part
 */
typedef struct s_complex {
    float real;
    float img;
} t_complex;

/**
 * s_range has every variable needed to change visualisation from fractals
 */
typedef struct s_range {
    float minX;
    float maxX;
    float minY;
    float maxY;
    int maxIter;
    float maxDeviation;
    t_complex unchanging;
    bool isMandelbrot;
} t_range;

/**
 * Color palette has "number_of_color" colors inside palette array, and a boolean to know if we should use
 * linear interpolation or not
 */
typedef struct s_color_palette {
    Uint32 *palette;
    unsigned short number_of_color:7;
    bool linear_interpolation:1;
} t_colors;

/**
 * Put a fractal on pWindow, with given parameters
 * @param pWindow The window to put fractal on
 * @param range a t_range object containing every parameters (range, divergence, ...)
 * @param colors a t_colors object containing fractal's colors
 * @return
 */
int fractals(SDL_Window *pWindow, t_range range, t_colors colors);

/**
 * Return a complex squared
 * @param nb Complex to square
 * @return nb * nb
 */
t_complex square(t_complex nb);

#endif //FRACTALES_FRACTAL_H
