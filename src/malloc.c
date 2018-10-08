#include "malloc.h"
#include <pthread.h>
#include "libr.h"

static pthread_mutex_t arena;

void show_alloc_mem()
{
}

extern void free(void *ptr)
{
    //ft_putfmt("free %p\n", ptr);
    if (ptr == NULL)
        return;
    pthread_mutex_lock(&arena);
    mmemalloc_free(ptr);
    pthread_mutex_unlock(&arena);
}

void *malloc(size_t size)
{
    void *ptr;

    //ft_putfmt("malloc %u\n", size);
    pthread_mutex_lock(&arena);
    ptr = mmemalloc_alloc(size);
    pthread_mutex_unlock(&arena);
    return (ptr);
}

void *realloc(void *ptr, size_t size)
{
    void *tmp;
    int res;

    //ft_putfmt("realloc %p of size %u\n", ptr, size);
    if (ptr == NULL)
    {
        //ft_putfmt("Malloc");
        return (malloc(size));
    }
    if (size == 0)
    {
        free(ptr);
        return (NULL);
    }
    pthread_mutex_lock(&arena);
    res = mmemalloc_expande(ptr, size);
    pthread_mutex_unlock(&arena);
    if (res == 1 || res == -1)
    {
        if (res == -1 || (tmp = malloc(size)) == NULL)
        {
            free(ptr);
            return (NULL);
        }
        ft_memcpy(tmp, ptr, ((t_memmagic *)ptr - 1)->size);
        free(ptr);
        return (tmp);
    }
    else
        return (ptr);
}

void *calloc(size_t nmemb, size_t size)
{
    //ft_putfmt("calloc %u %u\n", nmemb, size);
    void *ret;
    if (size == 0 || nmemb == 0)
        return (NULL);
    size = size * nmemb;
    if ((ret = malloc(size)) == NULL)
        return (NULL);
    bzero(ret, size);
    return (ret);
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
    //ft_putfmt("reallocarry %p %u %u\n", ptr, nmemb, size);
    return (realloc(ptr, nmemb * size));
}