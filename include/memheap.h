#ifndef ARENA_H
# define ARENA_H

# include "bhp.h"

# define AR_SMALL   4096
# define AR_ALIGN   16


/*
** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
** For the balance of ying and yang it is imperative that the
** two structures is the same size if no nothing will be like before
*/

typedef struct      s_memheap
{
    size_t          buffer_size;
    size_t          buffer_avail;
    void            *buffer;
    size_t          rc;
}                   t_memheap;

/*
** it is also imperative that root and buffer silk has the same place,
** if buffer is null then we are on a big chunk if no we are on a small
*/

typedef struct      s_memchunk
{
    size_t               size;
    size_t               align_pls;
    t_memheap            *root;
    struct s_memchunk    *next;
}                   t_memchunk;

/*
** Initialize a new empty heap by allocating buffer of size
** This heap can't be resized (see mh_init2)
** return 0 on success 1 on error
*/

int                 mh_init(t_memheap *arena, size_t size);

// Initialize a new empty heap by allocating buffer of size
// with PROT_GROWSDOWN, this heap can be resized
// same return value than mh_init

int                 mh_init2(t_memheap *arena, size_t size);

// Init heap of size aligned on getpagesize() and return buffer addr
// heap_avail is set to 0

void                *mh_init3(t_memheap *area, size_t size);

// Expande map if new_size is bigger than old_size, do nothing else
// return mremap return value or buffer addr

t_memheap            *mh_init4(size_t size);

void                 *mh_growdown(t_memheap *area, size_t new_size);

// return munmap buffer ...

int                 mh_free(t_memheap *area);
int                 mh_free4(t_memheap *area);

//Looking for avaible chunk, if no chunk found return NULL

void                *mh_reserv_chunk(t_memheap *arena, size_t size);

// Looking for chunk with addr = @ptr and set it size to 0

int                 mh_free_chunk(void *ptr);

size_t              get_size_align(size_t size, size_t alignement);

void                bh_dump(t_bh *heap);

#endif