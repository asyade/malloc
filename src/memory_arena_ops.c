#include "memory_arena.h"

t_mem_chunk     *arena_expande_chunk(t_mem_chunk *chunk, size_t size)
{
    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    if (chunk->real_size >= size)
    {
        arena_alloc_delta(chunk->arena, -chunk->user_size);
        chunk->user_size = size;
        arena_alloc_delta(chunk->arena, chunk->user_size);        
        return (chunk);
    }
    if (chunk->prev && chunk->prev->status == FREE && chunk->prev->real_size + chunk->real_size + sizeof(t_mem_chunk) >= size)
    {
        arena_alloc_delta(chunk->arena, -chunk->user_size);        
        chunk->prev->real_size += chunk->real_size + sizeof(t_mem_chunk);
        arena_alloc_delta(chunk->arena, -chunk->prev->real_size);        
        chunk->prev->status = USED;
        chunk->prev->next = chunk->next;
        if (chunk->next)
            chunk->next->prev = chunk->prev;
        memmove(chunk->prev + 1, chunk + 1, chunk->user_size);
        return (chunk);
    }
    return  (NULL);
}

t_mem_chunk     *chunk_append(t_mem_chunk *chunk, size_t size)
{
    t_mem_chunk *new;
    if (chunk->real_size < chunk->user_size * 2 + sizeof(t_mem_chunk) + size)
    {
        new = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->user_size);//Maybe incrase realloc prevision
    }
    else
        new = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->user_size * 2);
    new->real_size = chunk->real_size - (((size_t)new - (size_t)chunk));//+ sizeof(t_mem_chunk));
    chunk->real_size -= new->real_size + sizeof(t_mem_chunk);
    new->prev = chunk;
    new->arena = chunk->arena;
    new->next = chunk->next;
    if (new->next)
    {
        new->next->prev = new;    
    }
    chunk->next = new;
    return chunk_fill(new, size);
}