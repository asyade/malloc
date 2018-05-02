ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so

CC = gcc
SRC =	src/malloc.c \
	src/arena_malloc.c \
	src/arena_free.c \
	src/arena_realloc.c \
	src/memory_arena_alloc.c \
	src/memory_arena.c \
	src/memory_arena_debug.c \
	src/memory_arena_ops.c \
	src/memory_arena_utils.c 
OBJ =	$(SRC:.c=.o)

LFT = -L./libft -lft
LIBS = $(LFT) -pthread
INCLUDES= -I./include -I./libft/include

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft/
	$(CC) $(OBJ) $(LFT) -shared -fPIC -Wl,-soname,$(NAME) -o $(NAME)

%.o : %.c
	$(CC) $< -c -fPIC $(INCLUDES) $(LIBS) -o $@

debug : $(OBJ)
	make -C libft/
	$(CC) $(OBJ) $(LFT) -g3 -shared -fPIC -Wl,-soname,$(NAME) -o $(NAME)

%.o : %.c
	$(CC) $< -c -fPIC -g3 $(INCLUDES) $(LIBS) -o $@

.PHONY: clean fclean

clean:
	make -C libft/ clean
	rm -f src/*.o

fclean: clean
	make -C libft/ fclean
	rm -f libft_malloc_*.so

re: fclean all
