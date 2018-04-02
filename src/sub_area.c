#include "area.h"

int         sar_init(t_subarea *area, size_t heapsize)
{
    if (bh_init(&area->index) != 0 ||
        mh_init(&area->heap_container, AREA_HEAPCNT) != 0)
    return (1);
    area->heap_size = heapsize;
    return (0);
}

t_memheap     *sar_add_chunk(t_subarea *area)
{
    t_bhnode    node;
    t_memheap   *newheap;

    if ((node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap))) == NULL)
        return (NULL);
    newheap = (t_memheap *)node.content;
    if (mh_init(newheap, AREA_DEFAULT) != 0)
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
    printf("@Init new heap\n");
    if ((newheap = sar_add_chunk(area)) == NULL)
        return (NULL);
    return (mh_reserv_chunk(newheap, size));
}

int         sar_free_chunk_recursive(t_subarea *area, void *ptr, size_t cursor)
{
}

int         sar_free_chunk(t_subarea *area, void *ptr)
{
    sar_free_chunk_recursive(area, ptr, 0);
}
