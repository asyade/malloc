#include "area.h"

int         sarb_init(t_subarea *area)
{
    if (bh_init(&area->index) != 0 ||
        mh_init(&area->heap_container, AREA_HEAPCNT) != 0)
        return (1);
    return (0);
}

void        *sarb_get_chunk(t_subarea *area, size_t size)
{
    t_bhnode    node;
    t_memheap   *newheap;
    void        *retval;

    size = get_size_align(size, getpagesize());
    if ((node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap))) == NULL)
    {
        if (mh_growdown(&area->heap_container, (area->heap_container_size *= 2)) == NULL)
            return (NULL);
        node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap));
    }
    ////printf("COntent %p\n", node.content);
    newheap = (t_memheap *)node.content;
    retval = mh_init3(newheap, size);
    node.value.min = (size_t)newheap->buffer;
    node.value.max = (size_t)newheap->buffer + newheap->buffer_size;
    if (retval != NULL)
    {
        //printf("insert\n");
        bh_insert(&area->index, &node);
    }
    return (retval);
}

#include<stdio.h>
int         sarb_free_chunk(t_subarea *area, void *ptr)
{
    t_bhnode    *node;
    t_memheap   *heap;

    if ((node = bh_find(&area->index, (size_t)ptr)) == NULL)
    {
        //printf("big not found");
        return (1);
    }
    //printf("[%lx-%lx] for big %p\n", node->value.min, node->value.max, ptr);
    return (mh_free(node->content));
}
