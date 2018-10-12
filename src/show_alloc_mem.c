#include "libr.h"
#include "malloc.h"

void            show_tiny_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = allocator;
    if (a->range.max != SM_MAX)
        return ;
    ft_putfmt("TINY : 0x%x\n", a + 1);
}

void            show_med_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = allocator;
    if (a->range.max != BG_MAX)
        return ;
    ft_putfmt("SMALL : 0x%x\n", a + 1);
}

void            show_big_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = allocator;
    if (a->range.max != 140737353822224)
        return ;
    ft_putfmt("LARGE : 0x%x\n", a + 1);
}

void			show_alloc_mem()
{
    t_bheap		*heap;

    heap = mmemalloc_heap();
    bheap_iter(heap, show_tiny_alloc_mem, 0);
    bheap_iter(heap, show_med_alloc_mem, 0);
    bheap_iter(heap, show_big_alloc_mem, 0);
}
