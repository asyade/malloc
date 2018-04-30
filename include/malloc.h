#ifndef MALLOC_H
# define MALLOC_H

# include "memory_arena.h"
# include <stdlib.h>
# include <pthread.h>


# include <stdio.h>

# define SMALL_HEAP_BUFFER 1024*1024*4
# define MED_HEAP_BUFFER  1024*1024*16

# define MALLOC_SAFE    1

# define BIG_HEAP_MIN   1024


extern void        free(void *ptr);
extern void        *malloc_(size_t size);
extern void        *realloc(void *ptr, size_t size);
extern void        *calloc(size_t nmemb, size_t size);
extern void         *reallocarray(void *ptr, size_t nmemb, size_t size);

#endif
