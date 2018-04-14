#include "malloc.h"
#include "area.h"

t_area      *area()
{
    static int      area_init = 0;
    static t_area   area;

    if (area_init == 0)
    {
        ar_init(&area);
        area_init = 1;
    }
    return (&area);
}

void        free(void *ptr)
{
    mh_free_chunk(ptr);    
}

void        *malloc(size_t size)
{
    return (ar_get_chunk(area(), size));
}


void        *realloc(void *ptr, size_t size)
{
    return (ar_get_chunk(area(), size));
}

void *calloc(size_t nmemb, size_t size)
{
    void    *ptr;

    if (nmemb == 0 || size == 0)
        return (NULL);
    ptr = malloc(nmemb * size);    
    bzero(ptr, nmemb * size);
    return (ptr);
}