#include "memheap.h"

int                 mh_init(t_memheap *area, size_t size)
{
    t_memchunk *chk;
    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    area->buffer_size = size;
    area->buffer_avail = size;
    chk = (t_memchunk *)area->buffer;
    chk->next = 0;
    chk->size = 0;
    return (area->buffer == (void *)-1);
}

int                 mh_init2(t_memheap *area, size_t size)
{
    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | PROT_GROWSDOWN | MAP_ANONYMOUS, 0, 0);
    area->buffer_size = size;
    area->buffer_avail = size;
    return (area->buffer == (void *)-1);
}

size_t              get_size_align(size_t size, size_t alignement)
{
    if (size % alignement > 0)
        return (alignement * (1 + (size / alignement)));
    else
        return (alignement);
}

void                 *mh_growdown(t_memheap *area, size_t new_size)
{
    if (new_size <= area->buffer_size)
        return (area->buffer);
    return (area->buffer = mremap(area->buffer, area->buffer_size, new_size, MREMAP_MAYMOVE));
}

int                 mh_free_chunk(t_memheap *arena, void *ptr)
{
    t_memchunk       *chunk;

    chunk = (t_memchunk *)((size_t)ptr - sizeof(t_memchunk));
    if ((size_t)chunk < (size_t)arena->buffer)
    {
        printf("Chunk %p is not in range (%p-%p) (smaller)\n", chunk,arena->buffer,  (size_t)arena->buffer + arena->buffer_size);        
        return (2);
    }
    if ((size_t)chunk > (size_t)arena->buffer + arena->buffer_size)
    {
        printf("Chunk %p is not in range (%p-%p) (bigger)\n", chunk,arena->buffer,  (size_t)arena->buffer + arena->buffer_size);        
        return (1);
    }
    arena->buffer_avail += (size_t)chunk->next - (size_t)chunk;
    //printf("Size used = %zu\n", arena->buffer_size - arena->buffer_avail);
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
        (size_t)(((size_t)chunk - (size_t)arena->buffer)) < arena->buffer_size &&
        chunk->next != 0
        )
    {
        if (chunk->size == 0 && (size_t)chunk->next - (size_t)chunk - sizeof(t_memchunk) >= size) 
        {
            chunk->size = size;
            chunk->next = (t_memchunk *)((size_t)chunk + size + sizeof(t_memchunk));
            arena->buffer_avail -= ((size_t)chunk->next - (size_t)chunk);
            return ((void *)((size_t)chunk + sizeof(t_memchunk)));
        }
        chunk = chunk->next;
    }
    if ((size_t)((size_t)chunk + sizeof(t_memchunk) - (size_t)arena->buffer) + size <= arena->buffer_size) {
        chunk->size = size;
        chunk->next = (t_memchunk *)((size_t)chunk + size + sizeof(t_memchunk));
        arena->buffer_avail -= size + sizeof(t_memchunk);
        if ((size_t)chunk->next > (size_t)arena->buffer + arena->buffer_size)
            chunk->next = NULL;
        return ((void *)((size_t)chunk + sizeof(t_memchunk)));
    }
    return (NULL);
}