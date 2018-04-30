#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<unistd.h>
#include "malloc.h"



int rand_rang(int max, int min)
{
    return (rand() % (max + 1 - min) + min);
}

# define PTR_COUNT 16

void        randomize_array(void **array, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int     to =rand() % len;
        void    *tmp;

        tmp = array[to];
        array[to] = array[i];
        array[i] = tmp;
    }
}

int main(int ac, char **av)
{
    printf("%zu %zu\n", sizeof(t_mem_chunk), sizeof(t_mem_chunk));
    if (ac > 1)
        srand(atoi(av[1]));
    else
        srand(4242);


    void        *ptr[PTR_COUNT];


    for (int i = 0; i < PTR_COUNT; i++)
    {
        ptr[i] = malloc(rand_rang(10, 200));
        memset(ptr[i], 42, DPTR_TO_CHK(ptr[i])->user_size);
    }
    randomize_array(ptr, PTR_COUNT);
    for (int i = 0; i < PTR_COUNT / 2; i++)
        free(ptr[i]);
    for (int i =0; i< PTR_COUNT / 2; i++)
    {
        ptr[i] = malloc(16);
        memset(ptr[i], 42, 16);
    }
    for (int i = PTR_COUNT/2; i < PTR_COUNT; i++)
    {
        realloc(ptr[i], DPTR_TO_CHK(ptr[i])->user_size * 2);
        memset(ptr[i], 42, DPTR_TO_CHK(ptr[i])->user_size);
    }
    randomize_array(ptr, PTR_COUNT);
    for(int i = 0; i < PTR_COUNT; i++)
    {
        free(ptr[i]);
    }
}