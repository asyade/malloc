#include "area.h"

int         sarb_init(t_subarea *area)
{
    if (bh_init(&area->index) != 0)
        return (1);
    return (0);
}

void        *sarb_get_chunk(t_subarea *area, size_t size)
{
    t_bhnode    node;
    t_memheap   *newheap;

    size = get_size_align(size, getpagesize());
    node.content = mh_init4(size);
    newheap = (t_memheap *)node.content;
    if (newheap == NULL)
        return (NULL);
    node.value.min = (size_t)newheap->buffer;
    node.value.max = (size_t)newheap->buffer + newheap->buffer_size;
    bh_insert(&area->index, &node);
    return (newheap->buffer);

}
