NAME=fractales

CC=gcc
FLAGS= -Wall -Werror -Wextra
FLAGS_LINK= -lSDL2 -lm

OPTI ?= 0
ifeq ($(OPTI), 1)
	FLAGS += -Ofast -ffast-math -march=native
endif

VALGRIND_CHECK ?= 0
ifeq ($(VALGRIND_CHECK), 1)
	FLAGS += -g3
endif

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	FLAGS+= -g3 -fsanitize=address
endif


SRC_PATH=src/
SRC_NAME=main.c\
         fractal.c\
         error.c\
         screen_handling.c

SRC=$(addprefix $(SRC_PATH), $(SRC_NAME))

INCLUDE_PATH=include/

OBJS=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(FLAGS_LINK) $(FLAGS)

$(SRC_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -I$(INCLUDE_PATH) -c $< -o $@ $(FLAGS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all