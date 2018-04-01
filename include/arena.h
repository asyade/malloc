#ifndef ARENA_H
# define ARENA_H

# include "bhp.h"

# define AR_SMALL   4096
# define AR_ALIGN   16

typedef struct      s_srena
{
    t_bh            tree;
    void            *buffer;
    size_t          buffer_size;
    size_t          buffer_avail;
}                   t_arena;

typedef struct      s_archunk
{
    size_t              size;
    struct s_archunk    *next;
}                   t_archunk;

//return 0 on success
int                 ar_init(t_arena *arena, size_t size);
int                 ar_cleaar(t_arena *arena);


void                *ar_reserv_chunk(t_arena *arena, size_t size);
int                 ar_free_chunk(t_arena *arena, void *ptr);

#endif