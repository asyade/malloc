#include <stdlib.h>
#include <test.h>
#include <stdio.h>

# define NBR_ALLOC 40960

int       test_01(int seed)
{
    void    *allocs1[NBR_ALLOC];
    void    *allocs2[NBR_ALLOC];
    void    *allocs3[NBR_ALLOC];

    alloc_vec_rand(allocs1, NBR_ALLOC, 10, 512, seed);
    alloc_vec_rand(allocs2, NBR_ALLOC, 512, 1024, seed);
    alloc_vec_rand(allocs3, NBR_ALLOC, 1024, 4096, seed);

    if (ensure_vec(allocs1, NBR_ALLOC))
    {
        printf("Test 1-1 failed\n");
        return (1);
    }
    if (ensure_vec(allocs2, NBR_ALLOC))
    {
        printf("Test 1-2 failed\n");        
        return (1);
    }
    if (ensure_vec(allocs3, NBR_ALLOC))
    {
        printf("Test 1-3 failed\n");        
        return (1);    
    }

    free_vec(allocs1, NBR_ALLOC);
    free_vec(allocs2, NBR_ALLOC);
    free_vec(allocs3, NBR_ALLOC);

    return (0);
}