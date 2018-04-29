#include "malloc.h"

t_mem_arena        *get_small_heap(t_mem_arena *new)
{
    static t_mem_arena     *arena = NULL;

    if (new != NULL)
        arena = new;
    else if (arena == NULL)
    {
        if (!(arena = init_arena(SMALL_HEAP_BUFFER)))
            return (NULL);
        arena->next = arena;
        arena->prev = arena;
        arena->root = 1;
    }
    return (arena);
}

void        xfree(void *ptr)
{
    arena_free_chunk(DPTR_TO_CHK(ptr));
}

void        *xmalloc(size_t size)
{
    t_mem_chunk *tmp;
    t_mem_arena *area = get_small_heap(NULL);

    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    while (1)
    {
        if (area->buffer_size - area->buffer_used > size)
        {
            if((tmp = arena_get_chunk(size, area)))
                return (CHK_TO_DPTR(tmp));
        }
        area = area->next;
        if (area->root == 1)
            break ;
    }
    printf("No heap found");
    return (NULL);
}
