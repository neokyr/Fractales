cmake_minimum_required(VERSION 3.7)
project(Fractales C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2)
set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/x64/SDL2.lib;${PROJECT_SOURCE_DIR}/lib/x64/SDL2main.lib")
else ()
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/x86/SDL2.lib;${PROJECT_SOURCE_DIR}/lib/x86/SDL2main.lib")
endif ()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} include)

add_executable(Fractales
        include/error.h
        include/fractal.h
        include/screen_handling.h
        src/error.c
        src/fractal.c
        src/main.c
        src/screen_handling.c)

target_link_libraries(Fractales ${SDL2_LIBRARIES})