#include <stdlib.h>
#include <test.h>
#include <stdio.h>



# define NBR_ALLOC 128

int       test_03(int seed)
{
    void    *allocs1[NBR_ALLOC];

    alloc_vec_rand(allocs1, NBR_ALLOC, 1024*1024,1024*4096*16, seed);

    if (ensure_vec(allocs1, NBR_ALLOC))
    {
        printf("Test 3-1 failed\n");
        return (1);
    }
    realloc_vec_rand(allocs1, NBR_ALLOC,1024*4096*8,1024*4096*16*16, seed);

    if (ensure_vec(allocs1, NBR_ALLOC))
    {
        printf("Test 3-2 failed\n");
        return (1);
    }
    free_vec(allocs1, NBR_ALLOC);
    return (0);
}