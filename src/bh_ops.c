#include <bhp.h>

t_bhnode            *bh_find_recursive(t_bh *heap, size_t index, size_t value)
{
    size_t          left;
    size_t          right;
    t_bhnode        *ret;


    if ((left = BH_LCHILD(index)) <= heap->heap_size)
    {
       // //printf("left\n");
        if (value <= heap->buffer[left].value.max &&
            value >= heap->buffer[left].value.min)
            return (&heap->buffer[left]);
        if (heap->buffer[left].value.min < value)
        {
            if ((ret = bh_find_recursive(heap, left, value)))
                return (ret);
        }
    }
    if ((right = BH_RCHILD(index)) <= heap->heap_size)
    {
      //  //printf("right\n");
        
        if (value <= heap->buffer[right].value.max &&
            value >= heap->buffer[right].value.min)
            return (&heap->buffer[right]);
        if (heap->buffer[right].value.min < value)
        {
            if ((ret = bh_find_recursive(heap, right, value)))
                return (ret);
        }
    }
    return NULL;
}

t_bhnode            *bh_find(t_bh *heap, size_t value)
{
    ////printf("find\n");
    if (heap->heap_size == 0)
        return NULL;
    if (value <= heap->buffer[0].value.max &&
        value >= heap->buffer[0].value.min)
        return &heap->buffer[0];
    return (bh_find_recursive(heap, 0, value));
}

size_t               bh_insert(t_bh *heap, t_bhnode *node)
{
    if (bh_ensure_buffer(heap, sizeof(t_bhnode)) == -1)
    {
        //printf("Can't ensure buffer");
        return (-1);
    }
    return bh_filter_up(heap, *node, heap->heap_size++);
}

int                 bh_remove(t_bh *heap, size_t node)
{
    if (node < 0 || node >= heap->heap_size)
        return (-1);
    heap->heap_size--;
    if (heap->heap_size == 1)
    if (node == heap->heap_size)
        return 1;
    heap->buffer[node] = heap->buffer[heap->heap_size];
    bh_filter(heap, heap->buffer[node], node);
}