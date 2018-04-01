#include <bhp.h>

t_bhnode            *bh_find_recursive(t_bh *heap, size_t index, t_value value)
{
    size_t          left;
    size_t          right;
    t_bhnode        *ret;

    if ((left = (index * 2) + 1) <= heap->heap_size)
    {
        if (heap->buffer[left].value < value)
        {
            if ((ret = bh_find_recursive(heap, left, value)))
                return (ret);
        }
        if (heap->buffer[left].value == value)
            return (&heap->buffer[left]);
    }
    if ((right = (index * 2) + 2) <= heap->heap_size)
    {
        if (heap->buffer[right].value < value)
        {
            if ((ret = bh_find_recursive(heap, right, value)))
                return (ret);
        }
        if (heap->buffer[right].value == value)
            return (&heap->buffer[right]);
    }
    return NULL;
}

t_bhnode            *bh_find(t_bh *heap, t_value value)
{
    if (heap->buffer[0].value == value)
        return &heap->buffer[0];
    return (bh_find_recursive(heap, 0, value));
}

int                 bh_insert(t_bh *heap, t_bhnode *node)
{
    if (bh_ensure_buffer(heap, sizeof(t_bhnode)) == -1)
        return (-1);
    bh_filter_up(heap, *node, heap->heap_size++);
}

int                 bh_remove(t_bh *heap, size_t node)
{
    if (node < 0 || node >= heap->heap_size)
        return (-1);
    heap->heap_size--;
    if (node == heap->heap_size)
        return 1;
    heap->buffer[node] = heap->buffer[heap->heap_size];
    bh_filter(heap, heap->buffer[node], node);
}