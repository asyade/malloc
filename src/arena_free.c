#include "malloc.h"

void    arena_free(void *ptr, t_arena *root)
{
    t_mem_chunk     *chk;
    t_mem_arena     *arena;

    if (!ptr_is_valide(ptr))
        return ;
    chk = DPTR_TO_CHK(ptr);
    arena = chk->arena;
    if (arena_free_chunk(chk) < 0)
    {
        if (arena->status & BIG)
        {
            if (arena->prev)
                arena->prev->next = arena->next;
            if (arena->next)
                arena->next->prev = arena->prev;
            if (root->big == arena)
                root->big = arena->next;
            destroy_arena(arena);
            return ;
        }
        if (arena->next == arena->prev && arena->next == arena)
            return ;
        arena->next->prev = arena->prev;
        arena->prev->next = arena->next;
        destroy_arena(chk->arena);
    }
    else if (arena->status == BIG)
        destroy_arena(arena);
}
