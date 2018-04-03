#include "area.h"

int        ar_init(t_area *area)
{
    static size_t sizes[3] = {
        AREA_HEAP_SIZE_SMALL,
        AREA_HEAP_SIZE_MEDIUM,
        AREA_HEAP_SIZE_LARGE
    };
    int         i;
    size_t      page_size;

    page_size = getpagesize();
    sar_init(&area->subarea[0], AREA_HEAP_SIZE_SMALL * page_size);
    sar_init(&area->subarea[1], AREA_HEAP_SIZE_MEDIUM * page_size);
    sarb_init(&area->subarea[2]);
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
    if (i == 2)
        return sarb_get_chunk(&area->subarea[i], size);
    return sar_get_chunk(&area->subarea[i], size);
}

int      ar_free_chunk(t_area *area, void *ptr)
{//Todo find a better way
    int i;

    i = 0;
    while (i < AREA_SUB_COUNT - 1)
    {
        if (sar_free_chunk(&area->subarea[i++], ptr) == 0)
            return (0);
    }
    return (sarb_free_chunk(&area->subarea[2], ptr));
}