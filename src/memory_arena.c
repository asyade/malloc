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
    arena->max = (size_t)arena->buffer + size;
    chk = arena->buffer;
    chk->status = FREE;
    chk->size = 0;
    chk->next = NULL;
    chk->prev = NULL;
    chk->arena = arena;
    return (arena);
}

t_expstrat       chunk_get_strat(t_mem_chunk *chunk, size_t size)
{
    // If no next that means it's the last or first element so we can expande it to the arena size
    if (chunk->next == NULL && chunk->status != USED)
    {
        if (chunk->prev && (size_t)(chunk + 2) + size + chunk->prev->size < chunk->arena->max)
            return (chunk->status == MAY_USED ? SPLIT | ALTERNATELY : SPLIT);
        else if ((size_t)(chunk + 1) + size <= chunk->arena->max)
            return (DEF);
    }
    if (chunk->status == USED)
        return (NONE);
    if (chunk->size >= size)
    {
        return (chunk->status & MAY_USED ? DEF | ALTERNATELY : DEF);
    }
    return NONE;
}

/*
** Fill chunk with size
** if next chunk is free try alloc a chunk on the right side for realloc
*/
t_mem_chunk     *chunk_finalize(t_mem_chunk *chunk, size_t size)
{
    t_mem_chunk *tmp;

    chunk->size = size;
    chunk->status = USED;
    if (chunk->next == NULL && ((size_t)(chunk + 2) + size) < chunk->arena->max)
    {
        chunk->next = (t_mem_chunk *)((size_t)(chunk + 1) + size);
        printf("DIFF %zu\n", chunk->arena->max - (size_t)(chunk->next));
        chunk->next->size = chunk->arena->max - (size_t)(chunk->next + 1);
        printf("%zu size\n", chunk->next->size);
        chunk->next->arena = chunk->arena;
        chunk->next->prev = chunk;
        chunk->next->next = NULL;
        chunk->next->status = MAY_USED;//NEED TO BE MAY USED
        printf("%zu octet ready for realloc at %p\n", chunk->next->size, chunk->next);        
    }
    else if (chunk->next && (size_t)chunk->next - (MEM_ARENA_AL + sizeof(t_mem_chunk)) > (size_t)(chunk + 1) + size)
    {//@@@@@@@@IMPORTANT ici on ajoute MEM_ARENA_AL alors que si chunk ne remplis pas tout sons offset on a une perte de data definitve il faut ajouter le reste a size
        tmp = chunk->next;
        chunk->next = (t_mem_chunk *)((size_t)(chunk + 1) + size);
        chunk->next->prev = chunk;
        chunk->next->next = tmp;
        chunk->next->arena = chunk->arena;
        if (chunk->next->next) {
            chunk->next->size = (size_t)chunk->next->next - (size_t)(chunk->next + 1);
        }
        chunk->status = MAY_USED;
        printf("%p cell extruded of %p\n", chunk->next, chunk);
    }
    printf("%p finalized\n", chunk);
    return (chunk);
}

void            arena_free_chunk(t_mem_chunk *chunk)
{
    if (chunk->next && chunk->next->status != USED)
    {
        printf("fusion %p with %p (it's right neighbors)\n", chunk, chunk->next);
        chunk->size += chunk->next->size + sizeof(t_mem_chunk);
        chunk->next = chunk->next->next;
        if (chunk->next)
            chunk->next->prev = chunk;
    }

    if (chunk->prev && chunk->prev->status != USED)
    {
        printf("fusion %p with %p (it's left neighbors)\n", chunk, chunk->prev);
        chunk->prev->size += chunk->size + sizeof(t_mem_chunk);
        chunk->prev->next = chunk->next;
        if (chunk->next)
            chunk->next->prev = chunk->prev;
    }
    chunk->status = FREE;
    printf("%zu octet block avail\n", chunk->size);
}

/*
** Assuming we have size space left on our right create a new node at the right of chunk and finalize it
*/
t_mem_chunk     *chunk_append(t_mem_chunk *chunk, size_t size)///TODO remove useless
{
    chunk->next = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->size);
    chunk->next->next = NULL;
    chunk->next->prev = chunk;
    chunk->next->arena = chunk->arena;
    printf("Append to %p at %p\n", chunk, chunk->next);
    return (chunk_finalize(chunk->next, size));
}

t_mem_chunk     *chunk_split(t_mem_chunk *chunk, size_t size)
{
    t_mem_chunk *tmp;

    tmp = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->prev->size);
    if (chunk->next)
    {
        tmp->next = chunk->next;
        chunk->next->prev = tmp;
        tmp->next->prev = chunk;
    }
    else
        tmp->next = NULL;
    tmp->arena = chunk->arena;
    chunk->size = chunk->prev->size;
    chunk->next = tmp;
    printf("Spliting done\n");
    return chunk_finalize(chunk->next, size);
}

t_mem_chunk     *arena_fill_chunk(t_mem_chunk *chunk, size_t size, t_expstrat *strat)
{
    if (*strat & DEF)
        return (chunk_finalize(chunk, size));
    if (*strat & APPEND)
        return (chunk_append(chunk, size));
    if (*strat & SPLIT)
        return (chunk_split(chunk, size));
    printf("Unkonw strat\n");
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
    t_mem_chunk *current;
    t_expstrat  strat_alternative;
    t_expstrat  strat;

    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    current = arena->buffer;
    alternative = NULL;
    while (current)
    {
        if ((strat = chunk_get_strat(current, size)) != 0)
        {
            if (strat & ALTERNATELY)//FAT COMPLEXITY TODO trouver un moyens d'utiliser l'alternative sans tester tout les possibilitee avant
            {
                alternative = current;
                strat_alternative = strat;
            }
            else if ((ret = arena_fill_chunk(current, size, &strat)) != NULL)
                return (ret);
        }
        current = current->next;
    }
    return (alternative ? arena_fill_chunk(alternative, size, &strat_alternative) : NULL);
}

int             arena_expande_chunk_move(t_mem_chunk *chunk, size_t new_size)
{
    if ((size_t)chunk - new_size == 0)
        return (1);
    return (1);
}

t_mem_chunk     *arena_expande_chunk(t_mem_chunk *chunk, size_t new_size)
{
    new_size = SIZE_ALIGN(new_size, MEM_ARENA_AL);
    if (chunk->size >= new_size)
        return (chunk);
    if (chunk->next && chunk->next->status != USED && chunk->size + chunk->next->size + sizeof(t_mem_chunk) > new_size)
    {
        chunk->size += chunk->next->size + sizeof(t_mem_chunk);
        chunk->next = chunk->next->next;
        chunk->next->prev = chunk;
        printf("%p Expanded directly to %zu\n", chunk, new_size);
        return (chunk_finalize(chunk, new_size));
    }
    else if (arena_expande_chunk_move(chunk, new_size))
    {
        printf("Expande with move\n");
        return (chunk);        
    }
    return (NULL);
}