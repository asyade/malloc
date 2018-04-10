#include "malloc.h"
#include "area.h"

t_area      *area()
{
    static int      area_init = 0;
    static t_area   area;

    if (area_init == 0)
    {
        ar_init(&area);
    }
    return (&area);
}

#include<stdio.h>
void        free_(void *ptr)
{
    if (ar_free_chunk(area(), ptr) != 0)
    {
        printf("Cant free %p\n", ptr);
    }
}

void        *malloc_(size_t size)
{
    return (ar_get_chunk(area(), size));
}

void        *realloc(void *ptr, size_t size);

#include <stdio.h>
void show_alloc_mem()
{
    printf("TINY : 0x%p\n", &area()->subarea[0].heap_container);//todo
    
}
