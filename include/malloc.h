#ifndef MALLOC_H
# define MALLOC_H

# include "string.h"
# include "libr.h"

# define DEBUG_PREFIX CL_BLUE"[@ALLOCATOR@]"CL_RESET
# define ERROR_PREFIX CL_RED"[@ALLOCATOR@]"CL_RED
# define DEBUG_PAGE_INSERT  0
# define DEBUG_PAGE_DEL     0
# define DEBUG_PAGE_FAILED  0
# define DEBUG_PAGE_USED    0

extern void *calloc(size_t count, size_t size);
extern void free(void *ptr);
extern void *malloc(size_t size);
extern void *realloc(void *ptr, size_t size);
extern void *reallocf(void *ptr, size_t size);
extern void *valloc(size_t size);

extern void show_alloc_mem();

#endif
