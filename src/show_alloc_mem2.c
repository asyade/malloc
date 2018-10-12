#include "malloc.h"
#include "libr.h"

# define SHOW_ALLOC_2_NBCOL 128

void        inner_show_alloc_mem2(void *ptr)
{
    TMA     *a;
    
    a = ((t_allocator *)ptr)->allocator;
    
}

void        show_alloc_mem2()
{
    t_bheap *heap;

    heap = mmemalloc_heap();
    bheap_iter(heap, &inner_show_alloc_mem2, 0);
}