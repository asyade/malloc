#ifndef MALLOC_H
# define MALLOC_H

# include "memory_arena.h"
# include <stdlib.h>
# include <pthread.h>


# include <stdio.h>

# define SMALL_HEAP_BUFFER 1024*1024*8
# define MED_HEAP_BUFFER  1024*1024*16

# define MED_HEAP_MIN   1024
# define BIG_HEAP_MIN   4096

typedef struct      s_arena
{
    pthread_mutex_t lock;
    t_mem_arena     *small;
    t_mem_arena     *medium;
    t_mem_arena     *big;
}                   t_arena;


extern void        show_alloc_mem();
void        dump_arena(t_arena *arena);

int             ptr_is_valide(void *ptr);
t_area_status   get_heap_type(size_t size);
void            *arena_malloc(t_arena *arena, size_t size);
void            *arena_realloc(void *ptr, size_t size);
void            arena_free(void *ptr, t_arena *arena);

extern void        free(void *ptr);
extern void        *malloc_(size_t size);
extern void        *realloc(void *ptr, size_t size);
extern void        *calloc(size_t nmemb, size_t size);
extern void         *reallocarray(void *ptr, size_t nmemb, size_t size);

#endif
