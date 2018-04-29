#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include "malloc.h"


#define LEN 50000

#define PTR_COUNT 10000

int main()
{

for (int retry = 0; retry < 1; retry++)
{
    t_mem_chunk *ptrs[PTR_COUNT];
    for (int i = 0; i < PTR_COUNT; i++)
    {
        ptrs[i] = xmalloc(1024);
        if (ptrs[i] != NULL) 
        {
            memset(ptrs[i] + 1, 42, 1024);
            write(2, ptrs[i] + 1, 1024);
        }
        else {
            return (1);
        }
    }
    for (int i = 0; i < PTR_COUNT; i++)
    {
        if (ptrs[i])
        {
            xfree(ptrs[i]);
            ptrs[i] = NULL;
        }
    }    
}
/*    arena_free_chunk(ptrs[9]);
    arena_free_chunk(ptrs[2]);
  */  
/*
    arena_free_chunk(ptrs[2]);
    arena_free_chunk(ptrs[9]);
    arena_free_chunk(ptrs[1]);
    arena_free_chunk(ptrs[0]);
*/
}