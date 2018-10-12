#include "libr.h"
#include "malloc.h"

void            show_allocator(t_memalloc *allocator)
{
    size_t      i;
    size_t      a;
    size_t      b;
    t_mementry  *e;

    while (i < allocator->used_entries->size)
    {
        e = ((t_mementry *)BH_INDEX(allocator->used_entries, i));
        a = ((size_t)(e->addr + 1)) & SHM_MSK; 
        b = ((size_t)(e->addr + 1) + (e->size - (2 * sizeof(t_memmagic)))) & SHM_MSK; 
        ft_putfmt("0x%x - 0x%x : %d octets\n", a, b, e->size - (2 * sizeof(t_memmagic)));
        i++;
    }
}

void            show_tiny_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != SM_MAX)
        return ;
    ft_putfmt("TINY : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void            show_med_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != BG_MAX)
        return ;
    ft_putfmt("SMALL : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void            show_big_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != 140737353822224)
        return ;
    ft_putfmt("LARGE : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void            show_undef_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max == 140737353822224 ||
            a->range.max == SM_MAX ||
            a->range.max == BG_MAX)
        return ;
    ft_putfmt("UNDEF : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void			show_alloc_mem()
{
    t_bheap		*heap;

    heap = mmemalloc_heap();
    bheap_iter(heap, show_tiny_alloc_mem, 0);
    bheap_iter(heap, show_med_alloc_mem, 0);
    bheap_iter(heap, show_big_alloc_mem, 0);
    bheap_iter(heap, show_undef_alloc_mem, 0);
}
