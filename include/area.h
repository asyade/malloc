#ifndef POOL_H
# define POOL_H

# include "bhp.h"
# include "memheap.h"

# define AREA_MAX_ALLOC_SIZE    (1024 * 1024 * 64)

# define AREA_HEAP_SIZE_SMALL   (1024 * 16)
# define AREA_HEAP_SIZE_MEDIUM  (1024 * 64)
# define AREA_HEAP_SIZE_LARGE   (1024 * 1024)

# define AREA_SUB_COUNT         3


# define AREA_DEFAULT   
# define AREA_HEAPCNT   1024 * 64

# define MAX_AREA       4// Nbr of cpu core

typedef struct      s_subarea
{
    t_memheap       heap_container;
    t_bh            index;
    size_t          heap_size;
}                   t_subarea;

typedef struct      s_area
{
    t_subarea       subarea[AREA_SUB_COUNT];
}                   t_area;

int         ar_init(t_area *area);

void        *ar_get_chunk(t_area *area, size_t size);

int         ar_free_chunk(t_area *area, void *ptr);


int         sar_init(t_subarea *area, size_t heapsize);

void        *sar_get_chunk(t_subarea *area, size_t size);

int         sar_free_chunk(t_subarea *area, void *ptr);

#endif