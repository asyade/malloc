#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<unistd.h>
#include "malloc.h"


#define LEN 50000

#define PTR_COUNT 10
/*
int main()
{

    void    *ptr[3] = {malloc(560), malloc(128), malloc(1024)};
    
    free(ptr[0]);
    free(ptr[1]);
    free(ptr[2]);
}*/
/*
for (int retry = 0; retry < 1; retry++)
{
    t_mem_chunk *ptrs[PTR_COUNT];
    for (int i = 0; i < PTR_COUNT; i++)
    {
        ptrs[i] = malloc(1024*1024);
        if (ptrs[i] != NULL) 
        {
            memset(ptrs[i] + 1, 42, 1024);
        }
    }
    for (int i = 0; i < PTR_COUNT; i++)
    {
        if (ptrs[i])
        {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }    
}

}

*/