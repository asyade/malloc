#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include "area.h"

extern void        free(void *ptr);
extern void        *malloc_(size_t size);
extern void        *realloc(void *ptr, size_t size);
extern void        *calloc(size_t nmemb, size_t size);
void               show_alloc_mem();

#endif
