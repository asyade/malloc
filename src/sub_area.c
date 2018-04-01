#include "area.h"

int         sar_init(t_subarea *area, size_t heapsize)
{
    if (bh_init(&area->index) != 0)
        return (1);
    area->heap_size = heapsize;
    return (0);
}