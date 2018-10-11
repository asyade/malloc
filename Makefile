ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so

CC = gcc
SRC = src/malloc.c \
./src/malloc_fn2.c \
./src/binary_heap.c \
./src/binary_heap_fn2.c \
./src/rlib_fn_mem.c \
./src/rlib_fn_str.c \
./src/rlib_fn_str2.c \
./src/rlib_fn_str3.c \
./src/rlib.c \
./src/memory_chunk.c \
./src/memory_allocator.c \
./src/memory_allocator_fn2.c \
./src/memory_allocator_fn3.c \
./src/memory_allocator_ctor.c \
./src/memory_allocator_utils.c \
./src/memalloc_errno.c \
./src/thread_safe_allocator.c \
./src/managed_memory_allocator.c \
./src/managed_memory_allocator_fn2.c \
./src/managed_memory_allocator_fn3.c \

OBJ =	$(SRC:.c=.o)

LIBS = -pthread
INCLUDES= -I./include -fPIC

all: $(NAME) 

$(NAME): $(OBJ)
	$(CC) $(OBJ) -dynamic -shared -o $(NAME) -g3

%.o : %.c Makefile ./include/libr.h ./include/malloc.h
	$(CC) $< -c  $(INCLUDES) $(LIBS) -o $@ -g3

.PHONY: clean fclean

clean:
	rm -f src/*.o

fclean: clean
	rm -f libft_malloc_*.so

re: fclean all
