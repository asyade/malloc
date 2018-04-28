#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"malloc.h"
#include"test.h"
#define DEF_SEED 445538
#define TEST_COUNT 2//4

int (*tests[TEST_COUNT]) (int);

int main(int ac, char **av)
{
    tests[0] = &test_01;
    tests[1] = &test_02;
    tests[2] = &test_03;
    tests[3] = &test_04;
    int     seed = ac > 1 ? atoi(av[1]) : DEF_SEED;
    int     test = ac > 2 ? atoi(av[2]) : -1;
    srand(seed);

    if (test == -1)
    {
        for (int i = 0; i < TEST_COUNT; i++)
        {
            int ret = (*tests[i])(seed);
            if (ret)
                printf("Test %d/%d failed\n", i+1, TEST_COUNT);
            else
                printf("Test %d/%d success\n", i+1, TEST_COUNT);
        }       
    }
    else if (test <= TEST_COUNT && test > 0)
    {
        int ret = (*tests[test-1])(seed);
        if (ret)
            printf("Test %d failed\n", test);
        else
            printf("Test %d success\n", test);
    }
    else printf("usage test seed [test_number 1-%d]\n", TEST_COUNT);
}

size_t     rand_rang(size_t min, size_t max)
{
    return ((size_t)rand() % (max + 1 - min) + min);
}

void    alloc_vec_rand(void **vec, size_t nbr, size_t min, size_t max, unsigned int seed)
{
    for (size_t i = 0; i < nbr; i++)
    {
        vec[i] = malloc(rand_rang(min, max));
    }
}

void    realloc_vec_rand(void **vec, size_t nbr, size_t min, size_t max, unsigned int seed)
{
    srand(seed);
    for (size_t i = 0; i < nbr; i++)
    {
        vec[i] = realloc(vec[i], rand_rang(min, max));
    }
}


int     ensure_vec(void **vec, int nbr)
{
    for (size_t i = 0; i < nbr; i++)
    {
        if (vec[i] == NULL)
        {
            printf("Null addr at %ld\n", i);
            return (1);
        }
        if ((size_t)vec[i] % 16)
        {
            printf("Mem not aligned at %p:%ld\n", vec[i], i);
            return (1);
        }

        char tmp = *((unsigned char *)vec[i]);
        *((unsigned char *)vec[i]) = tmp;

    }
    return (0);
}

void     free_vec(void **vec, size_t nbr)
{
    for (size_t i = 0; i < nbr; i++)
    {
        free(vec[i]);
    }
}