CC=clang
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	FLAGS += -g3 -fsanitize=address
endif 

VALGRIND_CHECK ?=0
ifeq ($(VALGRIND_CHECK), 1)
	FLAGS += -g
endif

OPTI ?= 0
ifeq ($(OPTI), 1)
	FLAGS += -Ofast -ffast-math -march=native
endif

SRC_PATH=src/
SRC_NAME=main.c

SRC=$(addprefix $(SRC_PATH), $(SRC_NAME))

OBJS=$(SRC:.c=.o)
