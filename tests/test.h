#ifndef TEST_H
# define TEST_H
#include <string.h>

int    test_01(int seed);
int    test_02(int seed);
int    test_03(int seed);
int    test_04(int seed);

size_t     rand_rang(size_t min, size_t max);
int     ensure_vec(void **vec, int nbr);
void    free_vec(void **vec, size_t nbr);
void    realloc_vec_rand(void **vec, size_t nbr, size_t min, size_t max, unsigned int seed);
void    alloc_vec_rand(void **vec, size_t nbr, size_t min, size_t max, unsigned int seed);

#endif