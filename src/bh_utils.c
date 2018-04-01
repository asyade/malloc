#include <bhp.h>

int                bh_cmp(t_bhnode *a, t_bhnode *b)
{
    return (a->value < b->value);
}

void                bh_filter(t_bh *heap, t_bhnode node, size_t cursor)
{
    if (cursor > 0 && heap->buffer[cursor].value < heap->buffer[BH_PARENT(cursor)].value)
        bh_filter_up(heap, node, cursor);
    else
        bh_filter_down(heap, node, cursor);
}

void                bh_filter_up(t_bh *heap, t_bhnode node, size_t cursor)
{
    size_t          parent;

    parent = BH_PARENT(cursor);    
    while (cursor && bh_cmp(&node, &heap->buffer[parent]))
    {
        heap->buffer[cursor] = heap->buffer[parent];
        cursor = parent;
        parent = BH_PARENT(cursor);
    }
    heap->buffer[cursor] = node;
}

void                bh_filter_down(t_bh *heap, t_bhnode node, size_t cursor)
{
    size_t          left;
    size_t          right;

    left = BH_LCHILD(cursor);
    right = BH_RCHILD(cursor);
    if (
        left < heap->heap_size &&
        bh_cmp(heap->buffer[left].value, node.value) &&
        bh_cmp(heap->buffer[left].value, heap->buffer[right].value)
        )
    {
        heap->buffer[cursor] = heap->buffer[left];
        return bh_filter_down(heap, node, left);
    }
    else if (
        right < heap->heap_size &&
        bh_cmp(heap->buffer[right].value, node.value)
        )
    {
        heap->buffer[cursor] = heap->buffer[right];
        return bh_filter_down(heap, node, right);
    }
    heap->buffer[cursor] = node;
}

