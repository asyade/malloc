#include "area.h"

int         sar_init(t_subarea *area, size_t heapsize)
{
    if (bh_init(&area->index) != 0 ||
        mh_init(&area->heap_container, 1024) != 0)
    return (1);
    area->heap_size = heapsize;
    return (0);
}

void        *sar_get_chunck(t_subarea *area, size_t size)
{
    t_bhnode    node;

    if (area->index.heap_size <= 0) {
        node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap));
        bh_insert(&area->index, &node);
    }
}