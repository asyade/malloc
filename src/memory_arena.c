#include "malloc.h"
#include "memory_arena.h"

# include <stdio.h>

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
    arena->max = (size_t)(arena->buffer + 1) + arena->buffer_size;
    chk = arena->buffer;
    chk->status = FREE;
    chk->size = 0;
    chk->next = NULL;
    chk->prev = NULL;
    chk->arena = arena;
    return (arena);
}

t_expstrat       get_max_avail(t_mem_chunk *chunk, size_t size)
{
    printf("Get max avail\n");
    return NONE;
}

/*
**
**
*/
t_expstrat       chunk_get_strat(t_mem_chunk *chunk, size_t size)
{
    if ((chunk->status == USED || chunk->status == MAY_USED) &&
        chunk->next == NULL && (size_t)(chunk+2) + chunk->size + size <= chunk->arena->max)
            return APPEND;
    else if (chunk->status == USED)
        return (NONE);
    // If no next that means it's the last or first element so we can expande it to the arena size
    if (chunk->next == NULL)
        if ((size_t)chunk + size <= chunk->arena->max)
            return (chunk->status == MAY_USED ? EXPANDE | ALTERNATELY : EXPANDE);
    if (chunk->size >= size)
        return (chunk->status & MAY_USED ? DEF | ALTERNATELY : DEF);
    return get_max_avail(chunk, size);    
}

/*
** Fill chunk with size
** if next chunk is free try alloc a chunk on the right side for realloc
*/
t_mem_chunk     *chunk_finalize(t_mem_chunk *chunk, size_t size)
{
    chunk->size = size;
    chunk->status = USED;
    if (chunk->next == NULL && (size_t)(chunk + 1) + (size * 2) <= chunk->arena->max)
    {
        chunk->next = (t_mem_chunk *)((size_t)(chunk + 1) + size);
        if ((size_t)(chunk->next + 1) + size > chunk->arena->max) {
            printf("PANIC: invalide next\n");
        }
        chunk->next->size = size;
        chunk->next->arena = chunk->arena;
        chunk->next->prev = chunk;
        chunk->next->next = NULL;
        chunk->next->status = MAY_USED;
        printf("%zu octet ready for realloc at %p\n", size, chunk->next);
    }
    printf("%p finalized\n", chunk);
    return (chunk);
}

/*
** Assuming we have size space left on our right create a new node at the right of chunk and finalize it
*/
t_mem_chunk     *chunk_append(t_mem_chunk *chunk, size_t size)
{
    chunk->next = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->size);
    chunk->next->next = NULL;
    chunk->next->prev = chunk;
    chunk->next->arena = chunk->arena;
    printf("Append to %p at %p\n", chunk, chunk->next);
    return (chunk_finalize(chunk->next, size));
}

t_mem_chunk     *arena_fill_chunk(t_mem_chunk *chunk, size_t size, t_expstrat *strat)
{
    if (*strat & (DEF | EXPANDE))
        return (chunk_finalize(chunk, size));
    if (*strat & APPEND)
        return (chunk_append(chunk, size));
    return (NULL);
}

/*
** If the chunk is not usable skip all free chunk to te next used chunk
** and chek if it next is usable
** when a usable chunk is found return it with buffer of size allocated
** or return NULL
*/
t_mem_chunk     *arena_get_chunk(size_t size, t_mem_arena *arena)
{
    t_mem_chunk *ret;
    t_mem_chunk *alternative;
    t_expstrat  strat_alternative;
    t_expstrat  strat;
    int         step;

    t_mem_chunk *current;

    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    current = arena->buffer;
    step = 0;
    while (current)
    {
        if (step == 0)
        {
            if ((strat = chunk_get_strat(current, size)) != 0)
            {
                if (strat & ALTERNATELY)
                {
                    printf("%p alternative\n", current);
                    alternative = current;
                    strat_alternative = strat;
                }
                else if ((ret = arena_fill_chunk(current, size, &strat)) != NULL)
                    return (ret);
            }
            else
                step = 1;
        }
        if (step == 1 && current->status == USED)
            step = 0;
        current = current->next;
    }
    return (alternative ? arena_fill_chunk(alternative, size, &strat_alternative) : NULL);
}