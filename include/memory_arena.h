#ifndef MEMORY_ARENA_H
# define MEMORY_ARENA_H

# include <string.h>
# include <sys/mman.h>
# include <limits.h>

# define MMAP_NULL ((void *)-1)
# define MEM_ARENA_AL     16
# define SIZE_ALIGN(sz, al) (sz % al > 0 ? al * (1 + (sz / al)) : sz)

typedef enum    e_mem_status
{
    FREE,
    USED,
    MAY_USED,
}               t_mem_status;

typedef enum    e_expstrat
{
    NONE           =0x0 ,//(1<<0),
    DEF            =0x1 ,//(1<<1),
    EXPANDE        =0x2 ,//(1<<2),
    APPEND         =0x4 ,//(1<<3),
    LEFT           =0x8 ,//(1<<4),
    RIGHT          =0x10,//(1<<5),
    TWICE          =0x20,//(1<<6),
    ALTERNATELY    =0x40 //(1<<7)
}               t_expstrat;

typedef struct          s_mem_arena
{
    void                *buffer;    
    size_t              buffer_size;
    size_t              max;
}                       t_mem_arena;

typedef struct          s_mem_chunk
{
    t_mem_status        status;
    size_t              size;
    t_mem_arena         *arena;
    struct s_mem_chunk  *next;
    struct s_mem_chunk  *prev;
}                       t_mem_chunk;


t_mem_arena     *init_arena(size_t buffer_min_size);
t_mem_chunk     *arena_get_chunk(size_t size, t_mem_arena *arena);

#endif