#include "area.h"

int         sar_init(t_subarea *area, size_t heapsize)
{
    area->heap_container_size = AREA_HEAPCNT;
    if (bh_init(&area->index) != 0 ||
        mh_init2(&area->heap_container, AREA_HEAPCNT) != 0)
            return (1);
    area->heap_size = heapsize;
    return (0);
}
#include<stdio.h>
t_memheap     *sar_add_chunk(t_subarea *area)
{
    t_bhnode    node;
    t_memheap   *newheap;
    if ((node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap))) == NULL)
    {
        //printf("Can't allocate new chunk\n");

        if (mh_growdown(&area->heap_container, (area->heap_container_size *= 2)) == NULL)
        {
            //printf("Can't grows down\n");
            return (NULL);
        }
        if ((node.content = mh_reserv_chunk(&area->heap_container, sizeof(t_memheap))) == NULL)
        {
            //printf("Extend node content failure\n");
            return (NULL);
        }
    }
    ////printf("COntent %p\n", node.content);
    newheap = (t_memheap *)node.content;
    if (mh_init(newheap, area->heap_size) != 0)
        return (NULL);
    node.value.min = (size_t)newheap->buffer;
    node.value.max = (size_t)newheap->buffer + newheap->buffer_size;
    bh_insert(&area->index, &node);
    ////printf("New map\n");
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
    {
        return (NULL);
    }
    return (mh_reserv_chunk(newheap, size));
}

int         sar_free_chunk(t_subarea *area, void *ptr)
{
    t_bhnode    *node;
    t_memheap   *heap;
    int         retval;
    size_t      wtf;

    if (!ptr)
        return (1);
    if ((node = bh_find(&area->index, (size_t)ptr)) == NULL)
        return (1);
    //1//printf("[%lX-%lX] for %p\n", node->value.min, node->value.max, ptr);
    retval = mh_free_chunk(node->content, ptr);
    if (((t_memheap *)node->content)->rc == 0)
    {
        mh_free(node->content);
        mh_free_chunk(&area->heap_container, node->content);
        bh_remove(&area->index, ((size_t)node - (size_t)area->index.buffer) /sizeof(t_bhnode));
    }
    return (retval);
}