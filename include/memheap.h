#ifndef ARENA_H
# define ARENA_H

# include "bhp.h"

# define AR_SMALL   4096
# define AR_ALIGN   16

typedef struct      s_memheap
{
    t_bh            tree;//Wtf don't remove it xD
    size_t          buffer_size;
    size_t          buffer_avail;
    void            *buffer;
}                   t_memheap;

typedef struct      s_memchunk
{
    size_t              size;
    struct s_memchunk    *next;
}                   t_memchunk;

//return 0 on success
int                 mh_init(t_memheap *arena, size_t size);
int                 mh_clear(t_memheap *arena);

//Looking for avaible chunk, if no chunk found return NULL
void                *mh_reserv_chunk(t_memheap *arena, size_t size);

int                 mh_free_chunk(t_memheap *arena, void *ptr);

size_t              get_size_align(size_t size, size_t alignement);

#endif