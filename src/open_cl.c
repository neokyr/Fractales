//
// Created by p00ri on 15/01/2023.
//
#include "open_cl.h"
#include "error.h"
#include "fractal.h"

char* getSource() {
    return
        "typedef struct {\n"
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
        "  ns[id] = i;\n"
        "}\n"
        ;
}



int clInit(t_opencl * opencl) {
    cl_int clErrCode = 0;
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
    opencl->changing_cmx = NULL;
    opencl->unchanging_cmx = NULL;
    opencl->results = NULL;

    clErrCode = clGetPlatformIDs(1, &(opencl->platform), NULL);
    if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't get platform");

    clErrCode = clGetDeviceIDs(opencl->platform,
                               CL_DEVICE_TYPE_GPU, 1,
                               &(opencl->device), NULL);
    if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't get device");

    opencl->context = clCreateContext(NULL, 1,
                                      &(opencl->device), NULL,
                                      NULL, &clErrCode);
    if(opencl->context == NULL) return print_error(ERROR_OPEN_CL, "Can't create context");

    opencl->queue = clCreateCommandQueueWithProperties(opencl->context, opencl->device,
                                                       (cl_command_queue_properties) 0, &clErrCode);
    if(opencl->queue == NULL) return print_error(ERROR_OPEN_CL, "Can't open queue");

    char* source = getSource();
    opencl->calc_fractals = clCreateProgramWithSource(opencl->context, 1,
                                                      (const char**) &source, NULL,
                                                      &clErrCode);
    if(opencl->calc_fractals == NULL) return print_error(ERROR_OPEN_CL, "Can't create program");

    clErrCode = clBuildProgram(opencl->calc_fractals, 0, NULL, NULL, NULL, NULL);
    if(clErrCode != CL_SUCCESS) return print_error(ERROR_OPEN_CL, "Can't compile program");

    opencl->main_kernel = clCreateKernel(opencl->calc_fractals, "calc_fractal", &clErrCode);
    if(opencl->main_kernel == NULL) return print_error(ERROR_OPEN_CL, "Can't create kernel");


    return 0;
}

int passArgs(t_opencl *opencl, t_range *range, SDL_Surface surface,int* resultat) {
    int clErrCode = 0;
    int size = surface.h * surface.w;

    if(!opencl->isInit){
        clErrCode = clInit(opencl);
        if(clErrCode != 0) return clErrCode;
    }

    if(opencl->results != NULL){
        clErrCode = clReleaseMemObject(opencl->results);
        if(clErrCode != CL_SUCCESS && clErrCode != CL_INVALID_MEM_OBJECT)
            return print_error(ERROR_OPEN_CL, "Can't delete result buffer");
        opencl->results = NULL;
    }
    opencl->results = clCreateBuffer(opencl->context, CL_MEM_HOST_READ_ONLY,
                                     sizeof(int) * size, NULL, &clErrCode);




}

