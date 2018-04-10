#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include "area.h"

void        free_(void *ptr);
void        *malloc_(size_t size);
void        *realloc(void *ptr, size_t size);
void        show_alloc_mem();

#endif
