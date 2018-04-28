gcc src/* -I./include/ -I./libft -g3 -L./libft  ./libft/*.o tests/*.c -I./tests -lpthread && ./a.out $@
