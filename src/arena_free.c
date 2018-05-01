#include "malloc.h"

void    arena_free(void *ptr)
{
    t_mem_chunk     *chk;
    t_mem_arena     *arena;

    if (!ptr_is_valide(ptr))
        return ;
    chk = DPTR_TO_CHK(ptr);
    arena = chk->arena;
    if (arena_free_chunk(chk) < 0)
    {
        if (arena->next == NULL)
        {
            destroy_arena(arena);
            return ;
        }
        if (arena->next == arena->prev && arena->next == arena)
            return ;
        arena->next->prev = arena->prev;
        arena->prev->next = arena->next;
        destroy_arena(chk->arena);
    }
}