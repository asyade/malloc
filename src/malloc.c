#include "malloc.h"

pthread_mutex_t     malloc_mutex;

t_mem_arena        *get_small_heap(t_mem_arena *new)
{
    static t_mem_arena     *arena = NULL;

    if (new != NULL)
        arena = new;
    else if (arena == NULL)
    {
        if (!(arena = init_arena(SMALL_HEAP_BUFFER)))
            return (NULL);
        arena->next = arena;
        arena->prev = arena;
        arena->root = 1;
    }
    return (arena);
}

void        free(void *ptr)
{
    t_mem_chunk     *chk;
    t_mem_arena     *arena;

    chk = DPTR_TO_CHK(ptr);
    pthread_mutex_lock(&malloc_mutex);    
    arena = chk->arena;
    if (arena_free_chunk(chk) < 0)
    {
        if (arena->next == NULL)
        {
            destroy_arena(arena);
            pthread_mutex_unlock(&malloc_mutex);                
            return ;
        }
        if (arena->next == arena->prev && arena->next == arena)
        {
            pthread_mutex_unlock(&malloc_mutex);
            return ;
        }
        arena->next->prev = arena->prev;
        arena->prev->next = arena->next;
        destroy_arena(chk->arena);
    }
    pthread_mutex_unlock(&malloc_mutex);    
}

void        *malloc(size_t size)
{
    t_mem_chunk *tmp;
    t_mem_arena *area;

    if (size == 0)
        return (NULL);
    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    if (size >= BIG_HEAP_MIN)
    {
        if ((area = init_arena(size * 2)) == NULL || (tmp = arena_get_chunk(size, area)) == NULL)
            return (NULL);
        return (CHK_TO_DPTR(tmp));
    }
    pthread_mutex_lock(&malloc_mutex);
    area = get_small_heap(NULL);
    while (1)
    {
        if (area->buffer_size - area->buffer_used > size)
        {
            if((tmp = arena_get_chunk(size, area)))
            {
                if (area->root != 1)
                {
                    get_small_heap(NULL)->root = 0;
                    area->root = 1;
                    get_small_heap(area);
                }
                pthread_mutex_unlock(&malloc_mutex);                
                return (CHK_TO_DPTR(tmp));
            }
        }
        area = area->next;
        if (area->root == 1)
            break ;
    }
    if (!(area = init_arena(SMALL_HEAP_BUFFER)))
    {
        pthread_mutex_unlock(&malloc_mutex);
        return (NULL);
    }
    area->next = get_small_heap(NULL);
    area->next->root = 0;
    area->root = 1;
    area->prev = area->next->prev;
    area->next->prev = area;
    area->prev->next = area;
    get_small_heap(area);
    pthread_mutex_unlock(&malloc_mutex);    
    return (malloc(size));
}

void        *realloc(void *ptr, size_t size)
{
    t_mem_chunk *chk;
    t_mem_chunk *new;

    if (size == 0 || ptr == NULL)
        return (NULL);
    pthread_mutex_lock(&malloc_mutex);
    chk = DPTR_TO_CHK(ptr);
    if ((chk = arena_expande_chunk(chk, size)) != NULL)
    {
        pthread_mutex_unlock(&malloc_mutex);         
        return (CHK_TO_DPTR(chk));
    }
    chk = DPTR_TO_CHK(ptr);
    if ((new = malloc(size)) == NULL)
    {
        pthread_mutex_unlock(&malloc_mutex);            
        return (NULL);//todo voir si on free ou pas en cas d'echeque
    }
    memcpy(new, ptr, chk->user_size);
    free(ptr);
    pthread_mutex_unlock(&malloc_mutex);    
    return (CHK_TO_DPTR(new));
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
    size_t  old_size;

    if (ptr == NULL || nmemb == 0 || size == 0)
        return (NULL);
    old_size = DPTR_TO_CHK(ptr)->user_size;
    size *= nmemb;
    ptr = realloc(ptr, size);
    bzero((void *)((size_t)ptr + old_size), size - old_size);
    return (ptr);
}