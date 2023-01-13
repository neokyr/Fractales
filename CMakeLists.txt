cmake_minimum_required(VERSION 3.7)
project(Fractales C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/SDL2-2.26.2/Module)

if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_PATH "${PROJECT_SOURCE_DIR}\\SDL2-2.26.2\\x86_64-w64-mingw32")
else ()
    set(SDL2_PATH "${PROJECT_SOURCE_DIR}\\SDL2-2.26.2\\i686-w64-mingw32")
endif ()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} include/ ${PROJECT_SOURCE_DIR}/SDL2-2.26.2/x86_64-w64-mingw32/include)


add_executable(Fractales
        include/error.h
        include/fractal.h
        include/screen_handling.h
        src/error.c
        src/fractal.c
        src/main.c
        src/screen_handling.c)

target_link_libraries(Fractales ${SDL2_LIBRARY})