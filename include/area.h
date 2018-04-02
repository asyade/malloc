#ifndef POOL_H
# define POOL_H

# include "bhp.h"
# include "memheap.h"

# define AREA_DEFAULT   1024 * 128
# define AREA_HEAPCNT   1024 * 128

typedef struct      s_subarea
{
    t_memheap       heap_container;
    t_bh            index;
    size_t          heap_size;
}                   t_subarea;

int         sar_init(t_subarea *area, size_t heapsize);

void        *sar_get_chunk(t_subarea *area, size_t size);

int         sar_free_chunk(t_subarea *area, void *ptr);

#endif