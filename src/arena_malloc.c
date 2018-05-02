#include "malloc.h"

void    *arena_malloc_big(t_arena *arena, size_t size)
{
    t_mem_chunk *tmp;
    t_mem_arena *area;

    size = SIZE_ALIGN(size, getpagesize());
    if ((area = init_arena(size * 2, BIG)) == NULL || (tmp = arena_get_chunk(size, area)) == NULL)
        return (NULL);
    area->next = NULL;
    area->prev = NULL;
    if (arena->big == NULL)
        arena->big = area;
    else
    {
        arena->big->prev = area;
        area->next = arena->big;
        arena->big = area;
    }
    return (CHK_TO_DPTR(tmp));
}

void    *arena_malloc_or_expande(t_mem_arena **area_ptr, size_t size)
{
    t_mem_arena *area;
    t_mem_chunk *tmp;

    area = *area_ptr;
    while (1)
    {
        if (area->buffer_size - area->buffer_used > size &&
            (tmp = arena_get_chunk(size, area))) {
            if (!(area->status & ROOT))
            {
                (*area_ptr)->status &= ~ROOT;
                area->status |= ROOT;
                *area_ptr = area;
            }
            return (CHK_TO_DPTR(tmp));
        }
        area = area->next;
        if (area->status & ROOT)
            break ;
    }
    if (!(area = init_arena(SMALL_HEAP_BUFFER, ROOT | get_heap_type(size) == BIG ? BIG : MED)))
        return (NULL);
    area->status &= ~ROOT;
    area->next = *area_ptr;
    area->next->status = NONE;
    area->prev = area->next->prev;
    area->next->prev = area;
    area->prev->next = area;
    *area_ptr = area;
    return (arena_malloc_or_expande(area_ptr, size));
}

void    *arena_malloc(t_arena *arena, size_t size)
{
    t_mem_status    type;

    if (size == 0)
        return (NULL);
    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    type = get_heap_type(size);
    if (type == BIG)
        return (arena_malloc_big(arena, size));
    else if (type == MED)
    {
        if (arena->medium == NULL)
        {
            
            arena->medium = init_arena(MED_HEAP_BUFFER, MED | ROOT);
            arena->medium->next = arena->medium;
            arena->medium->prev = arena->medium;
        }
        return (arena_malloc_or_expande(&arena->medium, size));
    }
    else if (arena->small == NULL)
    {
        arena->small = init_arena(MED_HEAP_BUFFER, MED | ROOT);
        arena->small->next = arena->small;
        arena->small->prev = arena->small;
    }
    return (arena_malloc_or_expande(&arena->small, size));
}