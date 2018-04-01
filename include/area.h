#ifndef POOL_H
# define POOL_H

# include "bhp.h"
# include "memheap.h"

typedef struct      s_subarea
{
    t_bh            index;
    t_memheap       heap;
}                   t_subarea;

#endif