#include "area.h"
#include<stdio.h>

int         sar_init(t_subarea *area, size_t heapsize)
{
    if (bh_init(&area->index) != 0)
        return (1);
    area->heap_size = heapsize;
    return (0);
}

t_memheap     *sar_add_chunk(t_subarea *area)
{
    t_bhnode    node;
    t_memheap   *newheap;

    node.content = mh_init4(area->heap_size);
    if ((newheap = (t_memheap *)node.content) == NULL)
        return (NULL);
    node.value.min = (size_t)newheap->buffer;
    node.value.max = (size_t)newheap->buffer + newheap->buffer_size;
    bh_insert(&area->index, &node);
    return (t_memheap *)node.content;
}

void        *sar_get_chunk(t_subarea *area, size_t size)
{
    void        *ret;
    size_t      i;
    t_memheap   *newheap;
    t_bhnode    *nodes;

    size = get_size_align(size, AR_ALIGN);
    if (area->index.heap_size > 0)
    {
        i = 0;
        nodes = area->index.buffer;
        while (i < area->index.heap_size)
        {
            newheap = nodes[i++].content;
            if (newheap->buffer_avail >= size &&
                (ret = mh_reserv_chunk(newheap, size)) != NULL)
                    return (ret);
        }
    }
    if ((newheap = sar_add_chunk(area)) == NULL)
        return (NULL);
    return (mh_reserv_chunk(newheap, size));
}

void        *sar_expand_chunk(t_subarea *area, void *ptr, size_t size)
{
    
}
