#include "area.h"

int        ar_init(t_area *area)
{
    static size_t sizes[3] = {
        AREA_HEAP_SIZE_SMALL,
        AREA_HEAP_SIZE_MEDIUM,
        AREA_HEAP_SIZE_LARGE
    };
    int    i;

    i = -1;
    while (++i < AREA_SUB_COUNT)
        if (sar_init(&area->subarea[i], sizes[i]) != 0)
            return (1);
    return (0);
}

int     ar_which(size_t size)
{
    static t_value limits[AREA_SUB_COUNT] = {
        (t_value) {0, 128},
        (t_value) {128, 1024},
        (t_value) {1024, AREA_MAX_ALLOC_SIZE}
    };
    size = get_size_align(size, AR_ALIGN);
    int i = 0;
    while (i < AREA_SUB_COUNT)
    {
        if (size >= limits[i].min && size <= limits[i].max)
            return i;
        i++;
    }
    return (-1);
}

void    *ar_get_chunk(t_area *area, size_t size)
{
    int i;

    size = get_size_align(size, AR_ALIGN);
    if ((i = ar_which(size)) == -1)
        return (NULL);
    return sar_get_chunk(&area->subarea[i], size);
}

int      ar_free_chunk(t_area *area, void *ptr)
{//Todo find a better way
    int i;

    i = 0;
    while (i < AREA_SUB_COUNT)
    {
        if (sar_free_chunk(&area->subarea[i++], ptr) == 0)
            return (0);
    }
    return (1);
}