ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so

CC = gcc
SRC =	src/malloc.c

OBJ =	$(SRC:.c=.o)

LFT = -L../libr ../libr/libr
LIBS = $(LFT) -pthread
INCLUDES= -I./include -I../libr/include

all: $(NAME)

$(NAME): $(OBJ)
	make -C ../libr/
	$(CC) $(OBJ) $(LFT) -shared -fPIC -Wl,-soname,$(NAME) -o $(NAME)

%.o : %.c
	$(CC) $< -c -fPIC $(INCLUDES) $(LIBS) -o $@

debug : $(OBJ)
	make -C ../libr/
	$(CC) $(OBJ) $(LFT) -g3 -shared -fPIC -Wl,-soname,$(NAME) -o $(NAME)

%.o : %.c
	$(CC) $< -c -fPIC -g3 $(INCLUDES) $(LIBS) -o $@

.PHONY: clean fclean

clean:
	make -C ../libr/ clean
	rm -f src/*.o

fclean: clean
	make -C ../libr/ fclean
	rm -f libft_malloc_*.so

re: fclean all
