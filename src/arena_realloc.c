#include "malloc.h"

void        *arena_realloc(void *ptr, size_t size)
{
    t_mem_chunk *chk;
    t_mem_chunk *new;

    if (ptr == NULL)
        return (NULL);
    if (size == 0 || !ptr_is_valide(ptr))
        return (ptr);
    chk = DPTR_TO_CHK(ptr);
    if ((chk = arena_expande_chunk(chk, size)) != NULL)
        return (CHK_TO_DPTR(chk));
    return (NULL);
}

t_area_status   get_heap_type(size_t size)
{
    if (size >= BIG_HEAP_MIN)
        return (BIG);
    if (size >= MED_HEAP_MIN);
        return (MED);
    return (SMALL);
}

int        ptr_is_valide(void *ptr)
{
    t_mem_chunk *chk;

    chk = DPTR_TO_CHK(ptr);
    if (!ptr || (size_t)ptr % 2)
        return (0);
    return (1);
}
