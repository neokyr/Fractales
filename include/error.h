//
// Created by pad on 1/5/23.
//

#ifndef FRACTALES_ERROR_H
#define FRACTALES_ERROR_H

#define ERROR_ARG_NBR 1, "Error: Program require 3 arguments, height, width, fractal type \n", ""
#define ERROR_SDL_INIT 2, "Can't init SDL: (%)\n", SDL_GetError()
#define ERROR_SDL_WINDOW_NULL 3, "Can't create window: %\n", SDL_GetError()
#define ERROR_SDL_NO_SURFACE 4, "Can't get window surface: %\n", SDL_GetError()
#define ERROR_SDL_WINDOW_UPDATE 5, "Can't update window: %\n", SDL_GetError()
#define ERROR_NAN 6, "Error: \"%\" is not a valid number\n"
#define ERROR_CANT_ALLOC_MEMORY 7, "Error: can't allocate memory", ""
#define ERROR_INCORRECT_TYPE 8, "Error: \"%\" is not an available fractal type\n"
#define ERROR_OOB_DIMENSIONS 9, "Error: the selected dimensions for the window cannot display on your screen\n", ""

int print_error(int errcode, const char* err_msg, const char* context);

#endif //FRACTALES_ERROR_H
