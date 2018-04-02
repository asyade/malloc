#include "memheap.h"

int                 mh_init(t_memheap *arena, size_t size)
{
    arena->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    arena->buffer_size = size;
    arena->buffer_avail = size;
    return (arena->buffer == (void *)-1);
}

size_t              get_size_align(size_t size, size_t alignement)
{
    if (size <= alignement)
        return (alignement);
    return (alignement * ((size / alignement)));
}

int                 mh_free_chunk(t_memheap *arena, void *ptr)
{
    t_memchunk       *chunk;

    chunk = (t_memchunk *)((size_t)ptr - sizeof(t_memchunk));
    if ((size_t)chunk < (size_t)arena->buffer || (size_t)chunk >= (size_t)arena->buffer + arena->buffer_size)
        return (1);
    arena->buffer_avail += (size_t)chunk->next - (size_t)chunk;
    chunk->size = 0;
    return (0);
}

void                *mh_reserv_chunk(t_memheap *arena, size_t size)
{
    t_memchunk       *chunk;

    if (size > arena->buffer_avail)
        return (NULL);
    size = get_size_align(size, AR_ALIGN);
    chunk = (t_memchunk *)arena->buffer;
    while (
        (size_t)((void *)((size_t)chunk - (size_t)arena->buffer)) < arena->buffer_size &&
        chunk->next != 0
        )
    {
        if (chunk->size == 0 &&  (size_t)chunk->next - (size_t)chunk - sizeof(t_memchunk) >= size) 
        {
            arena->buffer_avail -= ((size_t)chunk->next - (size_t)chunk);
            chunk->size = size;
            chunk->next = (t_memchunk *)((size_t)chunk + size);
            return ((void *)((size_t)chunk + sizeof(t_memchunk)));
        }
        chunk = chunk->next;
    }
    if ((size_t)((size_t)chunk + sizeof(t_memchunk) - (size_t)arena->buffer) + size <= arena->buffer_size) {
        chunk->size = size;
        arena->buffer_avail -= size + sizeof(t_memchunk);
        chunk->next = (t_memchunk *)((size_t)chunk + size + sizeof(t_memchunk));
        if ((size_t)chunk->next > (size_t)arena->buffer + arena->buffer_size)
            chunk->next = NULL;
        return ((void *)((size_t)chunk + sizeof(t_memchunk)));
    }
    return (NULL);
}