#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include "memory_arena.h"
# include "bheap.h"

# include <stdio.h>

# define SMALL_HEAP_BUFFER 1024*1024*16// 1024*1024*4
# define MED_HEAP_BUFFER  1024*1024*16

void        *xmalloc(size_t size);
void        xfree(void *ptr);

/*extern void        free(void *ptr);
extern void        *malloc_(size_t size);
extern void        *realloc(void *ptr, size_t size);
extern void        *calloc(size_t nmemb, size_t size);
void               show_alloc_mem();
*/
#endif
