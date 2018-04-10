#include <bhp.h>

int                bh_cmp(t_bhnode *a, t_bhnode *b)
{
    //Todo check ok
    return (a->value.max < b->value.min);
   // return (a->value < b->value);
}

#include<stdio.h>
void                bh_dump(t_bh *heap)
{
    int             i;
    int             x;
    int             z;

    i = 0;
    x = 1;
    z = 1;
    printf("---- BEGIN DUMP ----\n");    
    printf("Heap size : %zu\n", heap->heap_size);
    while (i < heap->heap_size)
    {
        x = z;
        z = x * 2;
        while (x-- && i < heap->heap_size)
        {
            printf("[%p-%p], ", (void*)heap->buffer[i].value.min, (void*)heap->buffer[i].value.max);
            i++;
        }
        printf("\n");
    }
    printf("---- END DUMP ----\n");    
}

void                bh_filter(t_bh *heap, t_bhnode node, size_t cursor)
{
    if (cursor > 0 && bh_cmp(&heap->buffer[cursor], &heap->buffer[BH_PARENT(cursor)]))
        bh_filter_up(heap, node, cursor);
    else
        bh_filter_down(heap, node, cursor);
}

size_t                bh_filter_up(t_bh *heap, t_bhnode node, size_t cursor)
{
    size_t          parent;

    parent = BH_PARENT(cursor);    
    while (cursor && node.value.min < heap->buffer[parent].value.min)
    {
        heap->buffer[cursor] = heap->buffer[parent];
        cursor = parent;
        parent = BH_PARENT(cursor);
    }
    heap->buffer[cursor] = node;
    return cursor;
}

void                bh_filter_down(t_bh *heap, t_bhnode node, size_t cursor)
{//TODO check cmp
    size_t          left;
    size_t          right;

    left = BH_LCHILD(cursor);
    right = BH_RCHILD(cursor);
    if (
        left < heap->heap_size &&
        bh_cmp(&heap->buffer[left], &node) &&
        bh_cmp(&heap->buffer[left], &heap->buffer[right])
        )
    {
        heap->buffer[cursor] = heap->buffer[left];
        return bh_filter_down(heap, node, left);
    }
    else if (
        right < heap->heap_size &&
        bh_cmp(&heap->buffer[right], &node)
        )
    {
        heap->buffer[cursor] = heap->buffer[right];
        return bh_filter_down(heap, node, right);
    }
    heap->buffer[cursor] = node;
}

