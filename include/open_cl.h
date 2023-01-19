//
// Created by p00ri on 15/01/2023.
//

#ifndef FRACTALES_OPEN_CL_H
#define FRACTALES_OPEN_CL_H

#include "../OpenCL-SDK/external/OpenCL-Headers/CL/cl.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

/**
 * Complex number with an imaginary and real part
 */
typedef struct s_complex {
    cl_double real;
    cl_double img;
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
    int isMandelbrot;
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
 * Return a complex squared
 * @param nb Complex to square
 * @return nb * nb
 */
t_complex square(t_complex nb);

typedef struct open_cl {
    bool isInit;
    int old_size;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program calc_fractals;
    cl_kernel main_kernel;
    cl_mem results_mem;
    cl_int* results;
    cl_mem changing_mem;
    t_complex *changing;
    cl_mem unchanging_cmx;
    cl_mem isMandelbrot;
    cl_mem maxIter;
    cl_mem maxDeviation;
} t_opencl;

int clInit(t_opencl* opencl);


int findDeviation(t_opencl *opencl, t_range *range, SDL_Surface surface);

/**
 * Put a fractal on pWindow, with given parameters
 * @param pWindow The window to put fractal on
 * @param range a t_range object containing every parameters (range, divergence, ...)
 * @param colors a t_colors object containing fractal's colors
 * @param pCl OpenCL object containing context needed to communicate with GPU
 * @return
 */
int fractals(SDL_Window *pWindow, t_range range, t_colors colors, t_opencl *pCl);

#endif //FRACTALES_OPEN_CL_H
