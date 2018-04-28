#include <stdlib.h>
#include <test.h>
#include <stdio.h>

#define FROM 10
#define TO  100
#define PER 10

int       test_04(int seed)
{
    for (int i = 1; i < 1024;i++)
    {
        void    *all = malloc(i);
        if (all == NULL)
            return (1);
        void        *d[1024];
        alloc_vec_rand(d, 1024, 1, 4096*1024, 6565);
        if ((all = realloc(all, i * 2)) == NULL)
            return (1);
        memset(all, 42, i*2);
        if ((all = realloc(all, i * 3)) == NULL)
            return (1);
        memset(all, 42, i*3);            
        if ((all = realloc(all, i * 4)) == NULL)
            return (1);
        memset(all, 42, i*4);        
        if ((all = realloc(all, i * 5)) == NULL)
            return (1);
        if (ensure_vec(d, 1024) != 0)
            return (1);
        free_vec(d, 1024);
        memset(all, 42, i*5);            
        if ((all = realloc(all, i * 6)) == NULL)
            return (1);
        memset(all, 42, i*6);            
        if ((all = realloc(all, i * 7)) == NULL)
            return (1);
        memset(all, 42, i*7);            
    }
    return (0);
}