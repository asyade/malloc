#ifndef AREA_H
# define AREA_H

# include "bhp.h"
# include "memheap.h"
# include <unistd.h>

# define AREA_MAX_ALLOC_SIZE    (1024 * 1024 * 64)

# define AREA_HEAP_SIZE_SMALL   16
# define AREA_HEAP_SIZE_MEDIUM  64
# define AREA_HEAP_SIZE_LARGE   16//TODO

# define AREA_SUB_COUNT         3


# define AREA_DEFAULT   
# define AREA_HEAPCNT   4096

# define MAX_AREA       4// Nbr of cpu core

typedef struct      s_subarea
{
    t_memheap       heap_container;
    size_t          heap_container_size;
    t_bh            index;
    size_t          heap_size;
}                   t_subarea;

typedef struct      s_area
{
    t_subarea       subarea[AREA_SUB_COUNT];
    int             init;
}                   t_area;

int         ar_init(t_area *area);

void        *ar_get_chunk(t_area *area, size_t size);

int         ar_free_chunk(t_area *area, void *ptr);


int         sarb_init(t_subarea *area);

void        *sarb_get_chunk(t_subarea *area, size_t size);

int         sarb_free_chunk(t_subarea *area, void *ptr);


int         sar_init(t_subarea *area, size_t heapsize);

void        *sar_get_chunk(t_subarea *area, size_t size);

int         sar_free_chunk(t_subarea *area, void *ptr);

#endif