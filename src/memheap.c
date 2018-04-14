#include "memheap.h"


void    total_delta(size_t delta)
{
    static size_t   total_used = 0;

    total_used += delta;
    printf("total used %zu->%zu\n", total_used - delta , total_used);
}

int                 mh_init(t_memheap *area, size_t size)
{
    t_memchunk *chk;
    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (area->buffer !=  NULL) total_delta(size); else //printf("Can't mmap %zu octet\n", size);
    ////printf("Buffer %u, Size %zu\n", (size_t)area->buffer % 16, sizeof(t_memchunk));
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
    if (area->buffer !=  NULL) total_delta(size); else //printf("Can't mmap %zu octet\n", size);
    area->buffer_size = size;
    area->buffer_avail = size;
    area->rc = 0;    
    return (area->buffer == (void *)-1);
}

void                *mh_init3(t_memheap *area, size_t size)
{
    area->buffer = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (area->buffer !=  NULL) total_delta(size); else //printf("Can't mmap %zu octet\n", size);
    area->buffer_size = size;
    area->buffer_avail = size;
    area->rc = 0;    
    return (area->buffer);
}

int                 mh_free(t_memheap *area)
{
    if (munmap(area->buffer, area->buffer_size) == 0)
    {
    if (area->buffer !=  NULL) total_delta(-area->buffer_size);
        //printf("Map %p succesfully freed\n", area->buffer);
        return (0);
    }
    else
    {
        //printf("Can't munmap %p\n", area->buffer);
        return (1);
    }
}

size_t              get_size_align(size_t size, size_t alignement)
{
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

int                 mh_free_chunk(t_memheap *arena, void *ptr)
{
    t_memchunk       *chunk;

    chunk = (t_memchunk *)((size_t)ptr - sizeof(t_memchunk));
    if ((size_t)chunk < (size_t)arena->buffer)
        return (2);
    if ((size_t)chunk > (size_t)arena->buffer + arena->buffer_size)
        return (1);
    arena->buffer_avail += (size_t)chunk->next - (size_t)chunk;
    ////printf("Size used = %zu\n", arena->buffer_size - arena->buffer_avail);
    chunk->size = 0;
    arena->rc--;
    //printf("[%p] %d rc\n",arena, arena->rc);
    return (0);
}

void                *mh_reserv_chunk(t_memheap *arena, size_t size)
{
    t_memchunk       *chunk;

    if (size > arena->buffer_avail)
        return (NULL);
    //size = get_size_align(size, AR_ALIGN);
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
            arena->rc++;
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
        arena->rc++;
        return ((void *)((size_t)chunk + sizeof(t_memchunk)));
    }
    return (NULL);
}