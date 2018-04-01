#include "area.h"

int         sar_init(t_subarea *area, size_t heapsize)
{
    if (bh_init(&area->index) != 0)
        return (1);
    if (mh_init(&area->heap, heap_size) != 0)
        return (2);
    return (0);
}