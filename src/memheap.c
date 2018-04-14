#include "memheap.h"

#include <string.h>


void    total_delta(size_t delta)
{
    static size_t   total_used = 0;

    total_used += delta;
}

int                 mh_init(t_memheap *area, size_t size)
{
    t_memchunk *chk;

    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (area->buffer !=  (void *)-1) total_delta(size);
    area->buffer_size = size;
    area->buffer_avail = size;

    chk = (t_memchunk *)area->buffer;
    chk->next = 0;
    chk->size = 0;
    area->rc = 0;
    return (area->buffer == (void *)-1);
}

int                 mh_init2(t_memheap *area, size_t size)
{
    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | PROT_GROWSDOWN | MAP_ANONYMOUS, 0, 0);
    if (area->buffer !=  (void *)-1) total_delta(size);
    area->buffer_size = size;
    area->buffer_avail = size;
    area->rc = 0;    
    return (area->buffer == (void *)-1);
}

t_memheap           *mh_init4(size_t size)
{
    t_memheap   *area;
    t_memchunk  *chk;

    area = mmap(NULL, size + sizeof(t_memheap), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    area->buffer = area + sizeof(t_memheap);
    if (area == (void *)-1)
        return (NULL);
    area->buffer_size = size;
    area->buffer_avail = size;
    area->rc = 0;

    chk = (t_memchunk *)area->buffer;
    chk->next = 0;
    chk->size = 0;
    area->rc = 0;
    return (area);
}

int                 mh_free4(t_memheap *area)
{
    total_delta(-area->buffer_size);    
    if (munmap(area, area->buffer_size + sizeof(t_memheap)) == 0)
        return (0);
    else
        return (1);
}

int                 mh_free(t_memheap *area)
{
    total_delta(-area->buffer_size);
    if (munmap(area->buffer, area->buffer_size) == 0)
        return (0);
    else
        return (1);
}

size_t              get_size_align(size_t size, size_t alignement)
{//TODO avoid overflow
    if (size % alignement > 0)
        return (alignement * (1 + (size / alignement)));
    else
        return (size);
}

void                 *mh_growdown(t_memheap *area, size_t new_size)
{
    if (new_size <= area->buffer_size)
        return (area->buffer);
    area->buffer = mremap(area->buffer, area->buffer_size, new_size, MREMAP_MAYMOVE);
    area->buffer_size = new_size;
    return (area->buffer);
}

int                 mh_free_chunk(void *ptr)
{
    t_memchunk       *chunk;
    t_memheap        *arena;

    chunk = ptr - sizeof(t_memchunk);
    //Rember ...
    arena = chunk->root == NULL ? chunk : chunk->root;
    arena->buffer_avail += (size_t)chunk->next - (size_t)chunk;
    chunk->size = 0;
    arena->rc--;
    return (0);
}

void                *mh_reserv_chunk(t_memheap *arena, size_t size)
{
    t_memchunk       *chunk;

    if (size > arena->buffer_avail)
        return (NULL);
    chunk = (t_memchunk *)arena->buffer;
    while (
        (size_t)(((size_t)chunk - (size_t)arena->buffer)) < arena->buffer_size &&
        chunk->next != 0
        )
    {
        if (chunk->size == 0 && (size_t)chunk->next - (size_t)chunk - sizeof(t_memchunk) >= size) 
        {
            chunk->root = arena;
            chunk->size = size;
            chunk->next = (t_memchunk *)((size_t)chunk + size + sizeof(t_memchunk));
            arena->buffer_avail -= ((size_t)chunk->next - (size_t)chunk);
            arena->rc++;
            return ((void *)((size_t)chunk + sizeof(t_memchunk)));
        }
        chunk = chunk->next;
    }
    if ((size_t)((size_t)chunk + sizeof(t_memchunk) - (size_t)arena->buffer) + size <= arena->buffer_size) {
        chunk->size = size;
        chunk->root = arena;
        chunk->next = (t_memchunk *)((size_t)chunk + size + sizeof(t_memchunk));
        arena->buffer_avail -= size + sizeof(t_memchunk);
        if ((size_t)chunk->next > (size_t)arena->buffer + arena->buffer_size)
            chunk->next = NULL;
        arena->rc++;
        return ((void *)((size_t)chunk + sizeof(t_memchunk)));
    }
    return (NULL);
}