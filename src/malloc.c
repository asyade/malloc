#include "malloc.h"

static t_arena  arena;

void        show_alloc_mem()
{
    dump_arena(&arena);
}

void        free(void *ptr)
{
    pthread_mutex_lock(&arena.lock);
    arena_free(ptr, &arena);
    pthread_mutex_unlock(&arena.lock);
}

void        *malloc(size_t size)
{
    void    *ptr;

    pthread_mutex_lock(&arena.lock);
    ptr = arena_malloc(&arena, size);
    show_alloc_mem();
    pthread_mutex_unlock(&arena.lock);    
    return (ptr);
}

void        *realloc(void *ptr, size_t size)
{
   void     *tmp;

    pthread_mutex_lock(&arena.lock);
    tmp = arena_realloc(ptr, size);
    pthread_mutex_unlock(&arena.lock);    
    if (tmp)
        return (tmp);
    if (!(tmp = malloc(size)))
        return (NULL);
    if (!ptr)
        return (tmp);
    if (size > DPTR_TO_CHK(ptr)->user_size)
        memcpy(tmp, ptr, size - DPTR_TO_CHK(ptr)->user_size);
    else
        memcpy(tmp, ptr, size);    
    free(ptr);
    return (tmp);
}

void        *calloc(size_t nmemb, size_t size)
{
    void        *ret;
    if (size == 0 || nmemb == 0)
        return (NULL);
    size = size * nmemb;
    if ((ret = malloc(size)) == NULL)
        return (NULL);
    bzero(ret, size);
    return (ret);
}

void         *reallocarray(void *ptr, size_t nmemb, size_t size)
{
    if (size == 0 || !ptr_is_valide(ptr))
        return (ptr);
    pthread_mutex_lock(&arena.lock);
    ptr = arena_realloc(ptr, size);
    pthread_mutex_unlock(&arena.lock);    
    return (ptr);
}