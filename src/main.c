#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<unistd.h>
#include "malloc.h"


#define LEN 50000

#define PTR_COUNT 10

int main()
{

    void    *ptr[3] = {malloc(560), malloc(128), malloc(1024)};
    
    memset(ptr[0], 42, 560);
    memset(ptr[1], 42, 128);
    memset(ptr[2], 42, 1024);
    free(ptr[1]);
    free(ptr[2]);
    free(ptr[0]);
malloc(10);

    write(1, ptr[0], 560);
    write(1, ptr[1], 128);
    write(1, ptr[2], 1024);
}
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