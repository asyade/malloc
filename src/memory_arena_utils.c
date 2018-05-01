#include "memory_arena.h"

t_expstrat      chunk_get_strat(t_mem_chunk *chunk, size_t size)
{
    if (chunk->status == USED)
    {
        if (chunk->real_size >= chunk->user_size * 2 + (size * 2) + sizeof(t_mem_chunk))
            return (APPEND);
        if (chunk->real_size >= chunk->user_size * 2 + size + sizeof(t_mem_chunk))
            return (APPEND | ALTERNATELY);
        else if (chunk->real_size >= chunk->user_size + size + sizeof(t_mem_chunk))
            return APPEND | ALTERNATELY;
        return NONE;
    }
    else if (chunk->real_size >= size + sizeof(t_mem_chunk) + MEM_ARENA_AL ||
            chunk->real_size >= size + sizeof(t_mem_chunk))
        return (FILL);
    return NONE;
}

void            chunk_try_join_prev(t_mem_chunk *chunk)
{
    if (!chunk->prev)
        return ;
    chunk->prev->real_size += chunk->real_size + sizeof(t_mem_chunk);
    chunk->prev->next = chunk->next;
    if (chunk->next)
        chunk->next->prev = chunk->prev;
}

void            chunk_try_join_next(t_mem_chunk *chunk)
{
    if (!chunk->next || chunk->next->status == USED)
        return ;
    chunk->real_size += chunk->next->real_size + sizeof(t_mem_chunk);
    chunk->next = chunk->next->next;
    if (chunk->next)
        chunk->next->prev = chunk;
}

t_mem_chunk     *chunk_fill(t_mem_chunk *chunk, size_t size)
{
    chunk->user_size = size;
    chunk->status = USED;
    arena_alloc_delta(chunk->arena, chunk->user_size);
    return (chunk);
}
