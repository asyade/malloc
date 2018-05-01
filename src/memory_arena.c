#include "memory_arena.h"

t_mem_arena     *init_arena(size_t buffer_min_size)
{
    size_t      size;
    t_mem_chunk *chk;
    t_mem_arena *arena;

    size = buffer_min_size + sizeof(t_mem_arena);
    size = SIZE_ALIGN(size, getpagesize());
    arena = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (arena == MMAP_NULL)
        return (NULL);  
    arena->buffer_size = size - sizeof(t_mem_arena);
    arena->buffer = arena + 1;
    arena->next = NULL;
    arena->prev = NULL;
    chk = arena->buffer;
    chk->status = FREE;
    chk->user_size = 0;
    chk->real_size = arena->buffer_size - sizeof(t_mem_chunk);
    chk->next = NULL;
    chk->prev = NULL;
    chk->arena = arena;
    return (arena);
}

void            destroy_arena(t_mem_arena *arena)
{
    munmap(arena, arena->buffer_size + sizeof(t_mem_arena));
}

void            arena_alloc_delta(t_mem_arena *arena, long long int delta)
{
    if (delta > 0)
        arena->buffer_used += sizeof(t_mem_chunk);
    else
        arena->buffer_used -= sizeof(t_mem_chunk);
    arena->buffer_used += delta;
}

int            arena_free_chunk(t_mem_chunk *chunk)
{
    t_mem_arena *arena;

    arena = chunk->arena;
    if (chunk->status == FREE || chunk->arena == NULL)
        return (0);
    arena_alloc_delta(arena, -chunk->user_size);
    chunk->status = FREE;
    chunk_try_join_next(chunk);
    chunk_try_join_prev(chunk);
    if (arena->buffer_used == 0)
        return (-1);
    return (1);
}
