#ifndef POOL_H
# define POOL_H

# include "bhp.h"
# include "memheap.h"

typedef struct      s_subarea
{
    t_memheap       heap_container;
    t_bh            index;
    size_t          heap_size;
}                   t_subarea;

#endif