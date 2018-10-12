#include "libr.h"

void    		bheap_iter(t_bheap *heap, t_bheap_iter iter, size_t i)
{
    if (i > heap->size)
        return ;
    (iter)((void *)((size_t)(heap + 1) + (i * heap->elem_size)));
    if (BH_LEFT(i) < heap->size)
        bheap_iter(heap, iter, BH_LEFT(i));
    if (BH_RIGHT(i) < heap->size)
        bheap_iter(heap, iter, BH_RIGHT(i));
}