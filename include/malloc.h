#ifndef MALLOC_H
# define MALLOC_H

# include "string.h"

extern void *calloc(size_t count, size_t size);
extern void free(void *ptr);
extern void *malloc(size_t size);
extern void *realloc(void *ptr, size_t size);
extern void *reallocf(void *ptr, size_t size);
extern void *valloc(size_t size);

#endif
