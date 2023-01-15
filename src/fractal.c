//
// Created by pad on 1/10/23.
//

#include "../include/fractal.h"
#include "../include/screen_handling.h"
#include "../include/error.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../OpenCL-SDK/external/OpenCL-Headers/CL/cl.h"

uint32_t solve(t_complex c, t_range range, t_colors colors, t_complex z0);

uint32_t get_linear_interpolation(t_range range, t_colors colors, int nb_iter_dev){


	double offset_norm = (nb_iter_dev % (range.maxIter / (colors.number_of_color -1 )));
	//	printf("%d %% %d / %d = %f\n",nb_iter_dev, range.maxIter, (colors.number_of_color-1), offset_norm);
	offset_norm = offset_norm / (((double)range.maxIter / (double)(colors.number_of_color -1)));
	// printf("%f\n", offset_norm);
	int index_base_color = ((nb_iter_dev * colors.number_of_color )/ (range.maxIter+1));
	uint32_t color_1 = colors.palette[index_base_color];
	

	if ( nb_iter_dev == range.maxIter ){
		return(color_1);
	}	
	else {
	
	
		uint32_t color_2 = colors.palette[index_base_color + 1];
        uint32_t color_3;
		for (int i = 0; i < 4; i++){
			/*((char*) &color_1)[i] = ceil( ((char*) &color_1)[i] * (1 - offset_norm));
			((char*) &color_2)[i] = ceil( ((char*) &color_2)[i] * offset_norm);
            ((char*) &color_3)[i] = ((char*) &color_1)[i] + ((char*) &color_2)[i];*/
            double y0 = ((char*) &color_1)[i];
            double y1 = ((char*) &color_2)[i];
            double x0 = index_base_color;
            double x1 = index_base_color+1;
            double xp = (double) nb_iter_dev * (double) (colors.number_of_color -1) / (double) range.maxIter;

            double yp = y0 + ((y1-y0) / (x1-x0)) * (xp -x0);

            ((char*) &color_3)[i] = floor(yp);
		}
		return(color_3);
	}
}

uint32_t solve(t_complex c, t_range range, t_colors colors, t_complex z0) {
    int i;
    t_complex sqZ, z = z0;
    for (i = 0; i < range.maxIter; i++) {
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

    //Setup OpenCL
    cl_device_id device = 0;
    cl_context context = 0;
    cl_int err = 0;
    cl_context_properties properties=0;
    cl_platform_id platformId = 0;
    err = clGetPlatformIDs(1, &platformId, NULL);
    err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(&properties, 1, &device, NULL, NULL, &err);
    if(context == NULL) return print_error(ERROR_CANT_ALLOC_MEMORY);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, (cl_command_queue_properties) 0, &err);

    //Define the kernel
    char* source = {
            "kernel void calcSin(global float *data) {\n"
            "  int id = get_global_id(0);\n"
            "  data[id] = sin(data[id]);\n"
            "}\n"
    };

    float data[] = {10.0, 12.2, 22.34, 33, 44, 22, 15};
#define LENGTH 7
#define DATA_SIZE (LENGTH * sizeof(float))

    //Compile the kernel
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "calcSin", NULL);

    //Create the memory object
    cl_mem buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, DATA_SIZE, NULL, NULL);

    //Copy data to the input
    clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, DATA_SIZE, data, 0, NULL, NULL);

    //Execute kernel
    clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
    size_t global_dimensions[] = {LENGTH,0,0};
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_dimensions, NULL, 0, NULL, NULL);

    //Read back result
    clEnqueueReadBuffer(queue, buffer, CL_FALSE, 0, sizeof(cl_float)*LENGTH, data, 0, NULL, NULL);

    //Wait for everything to finish
    clFinish(queue);

    for (int i = 0; i < LENGTH; ++i) {
        printf("%f\n", data[i]);
    }

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

    result.real = nb.real * nb.real -(nb.img * nb.img);
    result.img = 2 * nb.real * nb.img;

    return result;
}
