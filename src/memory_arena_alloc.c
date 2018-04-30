#include "memory_arena.h"

t_mem_chunk     *arena_get_chunk(size_t size, t_mem_arena *arena)
{
    t_mem_chunk *current;
    t_expstrat  strat;

    t_mem_chunk *alternative_chunk;
    t_expstrat  alternative_strat;
    t_expstrat  tmp;

    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    current = arena->buffer;
    alternative_chunk = NULL;

    while (current)
    {
        if ((tmp = chunk_get_strat(current, size)) != NONE)
        {
            strat = tmp;
            if (strat & ALTERNATELY && (!alternative_chunk || CHK_AVAIL(alternative_chunk) >= CHK_AVAIL(current)))
            {
                alternative_chunk = current;
                alternative_strat = strat;
            }
            else
                return arena_process_strat(current, size, strat);
        }
        current = current->next;
    }
    DEBUG_LINE();    
    return (alternative_chunk ? arena_process_strat(alternative_chunk, size, alternative_strat) : NULL);
}


t_mem_chunk     *arena_process_strat(t_mem_chunk *chunk, size_t size, t_expstrat strat)
{
    if (strat & APPEND)
    {
        DEBUG_LINE();
        return chunk_append(chunk, size);
    }
    if (strat & FILL)
    {
        DEBUG_LINE();
        return chunk_fill(chunk, size);
    }
    return (NULL);
}