#ifndef MALLOC_H
# define MALLOC_H

# include "string.h"

extern void free(void *ptr);
extern void *malloc_(size_t size);
extern void *realloc(void *ptr, size_t size);
extern void *calloc(size_t nmemb, size_t size);
extern void *reallocarray(void *ptr, size_t nmemb, size_t size);

#endif
