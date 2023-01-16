//
// Created by p00ri on 15/01/2023.
//

#ifndef FRACTALES_OPEN_CL_H
#define FRACTALES_OPEN_CL_H

#include "../OpenCL-SDK/external/OpenCL-Headers/CL/cl.h"

typedef struct open_cl {
    bool isInit;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program calc_fractals;
    cl_kernel main_kernel;
    cl_mem results;
    cl_mem changing_cmx;
    cl_mem unchanging_cmx;
    cl_mem isMandelbrot;
    cl_mem maxIter;
    cl_mem maxDeviation;
} t_opencl;

int clInit(t_opencl* opencl);



#endif //FRACTALES_OPEN_CL_H
