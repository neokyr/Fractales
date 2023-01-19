//
// Created by p00ri on 15/01/2023.
//
#include "open_cl.h"
#include "error.h"

char *getSource() {
    return
            "typedef struct s_complex {\n"
            "  double real;\n"
            "  double img;\n"
            "} t_complex; \n"
            "\n"
            "t_complex square(t_complex nb) {\n"
            "  t_complex result;\n"
            "  result.real = nb.real * nb.real - nb.img * nb.img;\n"
            "  result.img  = 2 * nb.real * nb.img;\n"
            "  return result;\n"
            "}\n"
            "\n"
            "kernel void calc_fractal(global int* ns, global t_complex* changing, "
            "t_complex unchanging, int isMandelbrot, "
            "float maxDeviation, int maxIter) {\n"
            "  t_complex z, c, zSqr;\n"
            "  int id = get_global_id(0);\n"
            "  if(isMandelbrot){\n"
            "    z = unchanging;\n"
            "    c = changing[id];\n"
            "  }\n"
            "  else {\n"
            "    z = changing[id];\n"
            "    c = unchanging;\n"
            "  }\n"
            "  \n"
            "  int i;\n"
            "  for(i = 0 ; i < maxIter ; i++) {\n"
            "    zSqr = square(z);\n"
            "    z.real = zSqr.real + c.real;\n"
            "    z.img = zSqr.img + c.img;\n"
            "    if(fabs(z.real + z.img) > maxDeviation) break;\n"
            "  }\n"
            "  //ns[id] = id % maxIter;\n"
            "  ns[id] = i;\n"
            "}\n";
}


int clInit(t_opencl *opencl) {
    cl_int clErrCode = 0;
    opencl->old_size = -1;
    opencl->isInit = true;
    opencl->platform = NULL;
    opencl->device = NULL;
    opencl->context = NULL;
    opencl->queue = NULL;
    opencl->calc_fractals = NULL;
    opencl->main_kernel = NULL;
    opencl->maxIter = NULL;
    opencl->maxDeviation = NULL;
    opencl->isMandelbrot = NULL;
    opencl->changing_mem = NULL;
    opencl->unchanging_cmx = NULL;
    opencl->results_mem = NULL;
    opencl->results = NULL;
    opencl->changing = NULL;

    clErrCode = clGetPlatformIDs(1, &(opencl->platform), NULL);
    if (clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't get platform");

    clErrCode = clGetDeviceIDs(opencl->platform,
                               CL_DEVICE_TYPE_GPU, 1,
                               &(opencl->device), NULL);
    if (clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't get device");

    opencl->context = clCreateContext(NULL, 1,
                                      &(opencl->device), NULL,
                                      NULL, &clErrCode);
    if (opencl->context == NULL) return print_error(ERROR_OPEN_CL, "Can't create context");

    opencl->queue = clCreateCommandQueueWithProperties(opencl->context, opencl->device,
                                                       (cl_command_queue_properties) 0, &clErrCode);
    if (opencl->queue == NULL) return print_error(ERROR_OPEN_CL, "Can't open queue");

    char *source = getSource();
    opencl->calc_fractals = clCreateProgramWithSource(opencl->context, 1,
                                                      (const char **) &source, NULL,
                                                      &clErrCode);
    if (opencl->calc_fractals == NULL) return print_error(ERROR_OPEN_CL, "Can't create program");

    clErrCode = clBuildProgram(opencl->calc_fractals, 0, NULL, NULL, NULL, NULL);
    if (clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't compile program");

    opencl->main_kernel = clCreateKernel(opencl->calc_fractals, "calc_fractal", &clErrCode);
    if (opencl->main_kernel == NULL) return print_error(ERROR_OPEN_CL, "Can't create kernel");


    return 0;
}

int re_init_buffer(cl_mem *buffer, t_opencl opencl, size_t size, void *host_ptr, cl_mem_flags flags) {
    cl_int clErrCode = 0;
    if (*buffer != NULL) {
        clErrCode = clReleaseMemObject(*buffer);
        if (clErrCode != CL_SUCCESS && clErrCode != CL_INVALID_MEM_OBJECT)
            return print_error(ERROR_OPEN_CL, "Can't delete result buffer");
        *buffer = NULL;
    }
    *buffer = clCreateBuffer(opencl.context, flags, size, host_ptr, &clErrCode);
    if (*buffer == NULL) return print_error(ERROR_OPEN_CL, "Can't create result buffer");

    if(flags & CL_MEM_READ_ONLY || flags & CL_MEM_READ_WRITE ){
        clErrCode = clEnqueueReadBuffer(opencl.queue, *buffer,
                                        CL_FALSE, 0,
                                        size, host_ptr,
                                        0, NULL, NULL);
        if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't enqueue buffer read");
    }

    return 0;
}


int passArgs(t_opencl *opencl, t_range *range, SDL_Surface surface) {
    cl_int clErrCode = 0;
    int size = surface.h * surface.w;

    if (!opencl->isInit) {
        clErrCode = clInit(opencl);
        if (clErrCode != 0) return clErrCode;
    }

    if (opencl->old_size != size) {
        opencl->old_size = size;

        if (opencl->changing != NULL) free(opencl->changing);
        opencl->changing = malloc(sizeof(t_complex) * size);
        if (opencl->changing == NULL) return print_error(ERROR_CANT_ALLOC_MEMORY);

        if (opencl->results != NULL) free(opencl->results);
        opencl->results = malloc(sizeof(cl_int) * size);
        if (opencl->results == NULL) return print_error(ERROR_CANT_ALLOC_MEMORY);

    }

    clErrCode = re_init_buffer(&(opencl->results_mem),
                               *opencl,
                               sizeof(int) * size, &(opencl->results),
                               CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY);
    if (clErrCode != 0) return clErrCode;


    double yIncrement = (range->maxY - range->minY) / (double) surface.h;
    double xIncrement = (range->maxX - range->minX) / (double) surface.w;


    double y = range->minY;
    for (int i = 0; i < surface.h; ++i, y += yIncrement) {
        double x = range->minX;
        for (int j = 0; j < surface.w; ++j, x += xIncrement) {
            opencl->changing[i * surface.w + j].real = x;
            opencl->changing[i * surface.w + j].img = y;

        }
    }

    clErrCode = re_init_buffer(&(opencl->changing_mem),
                               *opencl,
                               sizeof(t_complex) * size,
                               opencl->changing,
                               CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY);
    if (clErrCode != 0) return clErrCode;

    clErrCode = re_init_buffer(&(opencl->unchanging_cmx),
                               *opencl,
                               sizeof(t_complex),
                               &(range->unchanging),
                               CL_MEM_READ_ONLY);
    if (clErrCode != 0) return clErrCode;

    clErrCode = re_init_buffer(&(opencl->isMandelbrot),
                               *opencl,
                               sizeof(t_complex),
                               &(range->unchanging),
                               CL_MEM_READ_ONLY);
    if (clErrCode != 0) return clErrCode;

    clErrCode = re_init_buffer(&(opencl->maxDeviation),
                               *opencl,
                               sizeof(t_complex),
                               &(range->maxDeviation),
                               CL_MEM_READ_ONLY);
    if (clErrCode != 0) return clErrCode;

    clErrCode = re_init_buffer(&(opencl->maxIter),
                               *opencl,
                               sizeof(t_complex),
                               &(range->maxIter),
                               CL_MEM_READ_ONLY);
    if (clErrCode != 0) return clErrCode;

    clErrCode = clSetKernelArg(opencl->main_kernel, 0,
                               sizeof(opencl->results_mem), &opencl->results_mem);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg0");

    clErrCode = clSetKernelArg(opencl->main_kernel, 1,
                               sizeof(opencl->changing_mem), &opencl->changing_mem);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg1");

    clErrCode = clSetKernelArg(opencl->main_kernel, 2,
                               sizeof(opencl->unchanging_cmx), &opencl->unchanging_cmx);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg2");

    clErrCode = clSetKernelArg(opencl->main_kernel, 3,
                               sizeof(opencl->isMandelbrot), &opencl->isMandelbrot);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg3");

    clErrCode = clSetKernelArg(opencl->main_kernel, 4,
                               sizeof(opencl->maxDeviation), &opencl->maxDeviation);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg4");

    clErrCode = clSetKernelArg(opencl->main_kernel, 5,
                               sizeof(opencl->maxIter), &opencl->maxIter);
    if(clErrCode != 0) return print_error(ERROR_OPEN_CL, "Can't init arg5");


    size_t global_dimensions[] = {size,0,0};
    clErrCode = clEnqueueNDRangeKernel(opencl->queue, opencl->main_kernel,
                           1, NULL, global_dimensions,
                           NULL, 0, NULL, NULL);
    if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't enqueue kernel execution");

    clErrCode = clEnqueueReadBuffer(opencl->queue, opencl->results_mem,
                                    CL_FALSE, 0, sizeof(cl_int) * size,
                                    opencl->results, 0, NULL, NULL);
    if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't enqueue read of result");

    clFinish(opencl->queue);

    return 0;
}
