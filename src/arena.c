#include "arena.h"

int                 ar_init(t_arena *arena, size_t size)
{
    if (bh_init(&arena->tree) != 0)
        return 1;
    arena->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    arena->buffer_size = size;
    arena->buffer_avail = size;
    bzero(arena, 16);
    return (arena->buffer == (void *)-1);
}

size_t              get_size_align(size_t size, size_t alignement)
{
    if (size <= alignement)
        return (alignement);
    return (alignement * ((size / alignement) + 1));
}

int                 ar_free_chunk(t_arena *arena, void *ptr)
{
    t_archunk       *chunk;

    chunk = (t_archunk *)((size_t)ptr - sizeof(t_archunk));
    if ((size_t)chunk < (size_t)arena->buffer || (size_t)chunk >= (size_t)arena->buffer + arena->buffer_size)
        return (1);
    chunk->size = 0;
    return (0);
}

void                *ar_reserv_chunk(t_arena *arena, size_t size)
{
    t_archunk       *chunk;

    if (arena->buffer_avail < size)
        return (NULL);
    size = get_size_align(size, AR_ALIGN);
    chunk = (t_archunk *)arena->buffer;
    while (
        (size_t)((void *)((void *)chunk - (void *)arena->buffer)) < arena->buffer_size &&
        chunk->next != 0
        )
    {
        if (chunk->size == 0 &&  (size_t)chunk->next - (size_t)chunk - sizeof(t_archunk) >= size) 
        {
            chunk->size = size;
            chunk->next = (t_archunk *)((size_t)chunk + size);
            return ((void *)((size_t)chunk + sizeof(t_archunk)));
        }
        chunk = chunk->next;
    }
    if ((size_t)((void *)chunk - arena->buffer) + size <= arena->buffer_size) {
        chunk->size = size;
        chunk->next = (t_archunk *)((size_t)chunk + size + sizeof(t_archunk));
        if ((void *)chunk->next > (void *)arena->buffer + arena->buffer_size)
            chunk->next = NULL;
        return ((void *)((size_t)chunk + sizeof(t_archunk)));
    }
    return (NULL);
}