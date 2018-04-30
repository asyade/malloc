#ifndef MEMORY_ARENA_H
# define MEMORY_ARENA_H

# include <string.h>
# include <sys/mman.h>
# include <unistd.h>
# include <limits.h>
# include "libft.h"

# define MMAP_NULL ((void *)-1)
# define MEM_ARENA_AL     16
# define SIZE_ALIGN(sz, al) (sz % al > 0 ? al * (1 + (sz / al)) : sz)

# define DPTR_TO_CHK(ptr)((t_mem_chunk *)ptr - 1)
# define CHK_TO_DPTR(chk)((void *)(chk + 1))
# define CHK_AVAIL(chk) (chk->real_size - chk->user_size)

#define DEBUG_LINE()     ;//(debugline((char *)__FILE__, __LINE__, (char *)__FUNCTION__))
#define DEBUG_ALLOC(chk) ;//debugalloc(chk);
#define DEBUG_ARENA(chk) ;//debugarena(chk);
//# define DEBUG_LINE() (printf("@ %s:%d in function `%s`\n",  __FILE__, __LINE__ ,__FUNCTION__))

typedef enum    e_mem_status
{
    FREE,
    USED,
}               t_mem_status;

typedef enum    e_expstrat
{
    NONE           = 0x0,
    APPEND    = 0x2,
    FILL           = 0x8,
    ALTERNATELY    = 0x40
}               t_expstrat;

/*
** Il faudra faire un buffer circulaire d'area
** Si une area ne reussit pas a allouer on la met en bas
*/
typedef struct          s_mem_arena
{
    void                *buffer;    
    size_t              buffer_size;
    size_t              buffer_used;
    struct s_mem_arena  *next;
    struct s_mem_arena  *prev;
    int                 root;
    char                padding[8];
}                       t_mem_arena;

typedef struct          s_mem_chunk
{
    t_mem_status        status;
    size_t              user_size;
    size_t              real_size;
    t_mem_arena         *arena;
    struct s_mem_chunk  *next;
    struct s_mem_chunk  *prev;
}                       t_mem_chunk;

void    debugline(char *fname, int nbr, char *fn);
void    debugarena(t_mem_arena *arena);
void    debugalloc(t_mem_chunk *chk);

t_mem_chunk     *arena_get_chunk(size_t size, t_mem_arena *arena);
t_expstrat      chunk_get_strat(t_mem_chunk *chunk, size_t size);
t_mem_chunk     *arena_process_strat(t_mem_chunk *chunk, size_t size, t_expstrat strat);
t_mem_chunk     *chunk_fill(t_mem_chunk *chunk, size_t size);
t_mem_chunk     *chunk_append(t_mem_chunk *chunk, size_t size);

t_mem_chunk     *arena_expande_chunk(t_mem_chunk *chunk, size_t new_size);

int             arena_free_chunk(t_mem_chunk *chunk);
void            chunk_try_join_prev(t_mem_chunk *chunk);
void            chunk_try_join_next(t_mem_chunk *chunk);

t_mem_arena     *init_arena(size_t buffer_min_size);
void            destroy_arena(t_mem_arena *arena);

#endif