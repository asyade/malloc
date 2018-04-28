#include <stdlib.h>
#include <test.h>
#include <stdio.h>



# define NBR_ALLOC 1024

int       test_02(int seed)
{
    void    *allocs1[NBR_ALLOC];
    void    *allocs2[NBR_ALLOC];
    void    *allocs3[NBR_ALLOC];

    alloc_vec_rand(allocs1, NBR_ALLOC, 10, 512, seed);
    alloc_vec_rand(allocs2, NBR_ALLOC, 512, 1024, seed);
    alloc_vec_rand(allocs3, NBR_ALLOC, 1024, 4096, seed);

    if (ensure_vec(allocs1, NBR_ALLOC))
    {
        printf("Test 2-1 failed\n");
        return (1);
    }
    if (ensure_vec(allocs2, NBR_ALLOC))
    {
        printf("Test 2-2 failed\n");        
        return (1);
    }
    if (ensure_vec(allocs3, NBR_ALLOC))
    {
        printf("Test 2-3 failed\n");        
        return (1);    
    }

    realloc_vec_rand(allocs1, NBR_ALLOC, 512, 1024, seed);
    realloc_vec_rand(allocs2, NBR_ALLOC, 1024, 4096, seed);
    realloc_vec_rand(allocs3, NBR_ALLOC, 4096, 8600, seed);

    if (ensure_vec(allocs1, NBR_ALLOC))
    {
        printf("Test 2-1 failed\n");
        return (1);
    }
    if (ensure_vec(allocs2, NBR_ALLOC))
    {
        printf("Test 2-2 failed\n");        
        return (1);
    }
    if (ensure_vec(allocs3, NBR_ALLOC))
    {
        printf("Test 2-3 failed\n");        
        return (1);    
    }

    free_vec(allocs1, NBR_ALLOC);
    free_vec(allocs2, NBR_ALLOC);
    free_vec(allocs3, NBR_ALLOC);

    return (0);
}