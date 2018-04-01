#include "bhp.h"

int                 bh_init(t_bh *heap)
{
    t_bhnode        root;

    heap->buffer = mmap(NULL, BH_CHKSZ, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | PROT_GROWSDOWN, 0, 0);
    if (heap->buffer == (void *)-1)
        return (-1);
    heap->buffer_size = BH_CHKSZ;
    heap->heap_size = 0;
    return (0);
}

int                bh_clear(t_bh *heap)
{
    return (munmap(heap->buffer, heap->buffer_size));
}

int                 bh_ensure_buffer(t_bh *heap, size_t needed)
{
    if (heap->heap_size * sizeof(t_bhnode) + needed > heap->buffer_size) {
        heap->buffer = mremap(heap->buffer, heap->buffer_size, heap->buffer_size + BH_CHKSZ, MREMAP_MAYMOVE);
        heap->buffer_size += BH_CHKSZ;
        if (heap->buffer == (void *) -1)
            return -1;
        return 1;
    }
    return 0;
}
