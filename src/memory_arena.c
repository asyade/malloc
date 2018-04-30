#include "malloc.h"
#include "memory_arena.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd = -1;

void    debugline(char *fname, int nbr, char *fn)
{
    char buffer[4096];

    if (fd == -1)
        fd = open("./malloc_trace.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ft_putstr_fd(fname, fd);
    ft_putchar_fd(':', fd);
    ft_putnbr_fd(nbr, fd);
    ft_putstr_fd(" in fn ", fd);
    ft_putstr_fd(fn, fd);
    ft_putstr_fd("\n", fd);
}

void    debugalloc(t_mem_chunk *chk)
{
    ft_putnbrbase_fd((size_t)chk, "0123456789ABCDEF", fd);
    ft_putstr_fd(" IS ", fd);
    ft_putstr_fd(chk->status == FREE ? "FREE":"USED",fd);
    ft_putstr_fd(" USER ", fd);
    ft_putnbrbase_fd(chk->user_size, "0123456789", fd);
    ft_putstr_fd(" PREV ", fd);
    ft_putnbrbase_fd(chk->prev_size, "0123456789", fd);
    ft_putstr_fd(" REAL ", fd);
    ft_putnbrbase_fd(chk->real_size, "0123456789", fd);
    ft_putendl_fd("", fd);
}


t_mem_arena     *init_arena(size_t buffer_min_size)
{
    size_t      size;
    t_mem_chunk *chk;
    t_mem_arena *arena;

    size = buffer_min_size + sizeof(t_mem_arena);
    size = SIZE_ALIGN(size, getpagesize());
    
    arena = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (arena == MMAP_NULL)
        return (NULL);
    
    arena->buffer_size = size - sizeof(t_mem_arena);
    arena->buffer = arena + 1;
    arena->next = NULL;
    arena->prev = NULL;
    
    chk = arena->buffer;
    chk->status = FREE;
    chk->user_size = 0;
    chk->prev_size = 0;
    chk->real_size = arena->buffer_size - sizeof(t_mem_chunk);
    chk->next = NULL;
    chk->prev = NULL;
    chk->arena = arena;
    return (arena);
}

void            destroy_arena(t_mem_arena *arena)
{
    munmap(arena, arena->buffer_size + sizeof(t_mem_arena));
}

void            arena_alloc_delta(t_mem_arena *arena, long long int delta)
{
    if (delta > 0)
        arena->buffer_used += sizeof(t_mem_chunk);
    else
        arena->buffer_used -= sizeof(t_mem_chunk);
    arena->buffer_used += delta;
}

/*
** APPEND_LARGE get a new block from free space, keep prev_size of existing block and prev_size for the new block is avaible
** APPEND_LARGE | ALTERNATLY do not keep prev_size for existing block
** APPEND_FILL get a new block from free space, keep prev_size of existing block but not prev_size is avaible for the new block
** APPEND_FILL | ALTERNATLY do not kepp prev_size for existing block
** FILL fill all the space into the current block
** FILL_LARGE fill space and take prev_size
*/

t_mem_chunk     *arena_get_chunk(size_t size, t_mem_arena *arena)
{
    DEBUG_LINE();
    t_mem_chunk *current;
    t_expstrat  strat;

    t_mem_chunk *alternative_chunk;
    t_expstrat  alternative_strat;
    t_expstrat  tmp;

    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    current = arena->buffer;
    alternative_chunk = NULL;

    while (current)
    {
        if ((tmp = chunk_get_strat(current, size)) != NONE)
        {
            strat = tmp;
            if (strat & ALTERNATELY && (!alternative_chunk || CHK_AVAIL(alternative_chunk) >= CHK_AVAIL(current)))
            {
                alternative_chunk = current;
                alternative_strat = strat;
            }
            else
                return arena_process_strat(current, size, strat);
        }
        current = current->next;
    }
    return (alternative_chunk ? arena_process_strat(alternative_chunk, size, alternative_strat) : NULL);
}

t_expstrat       chunk_get_strat(t_mem_chunk *chunk, size_t size)
{
    if (chunk->status == USED)
    {
        if (chunk->real_size >= chunk->prev_size + size + sizeof(t_mem_chunk) + MEM_ARENA_AL)
            return APPEND_LARGE;
        if (chunk->real_size >= chunk->prev_size + size + sizeof(t_mem_chunk))
            return APPEND_FILL;
        if (chunk->real_size >= chunk->user_size + size + sizeof(t_mem_chunk) + MEM_ARENA_AL)
            return APPEND_LARGE | ALTERNATELY;
        if (chunk->real_size >= chunk->user_size + size + sizeof(t_mem_chunk))
            return APPEND_FILL | ALTERNATELY;
        else
            return NONE;
    }
    if (chunk->real_size >= size + sizeof(t_mem_chunk) + MEM_ARENA_AL)
        return (FILL_LARGE);
    else if (chunk->real_size >= size + sizeof(t_mem_chunk))
        return (FILL);
    return NONE;
}

t_mem_chunk     *arena_process_strat(t_mem_chunk *chunk, size_t size, t_expstrat strat)
{
    if (strat & (APPEND_LARGE | APPEND_FILL))
        return chunk_append(chunk, size, strat);
    if (strat & (FILL | FILL_LARGE))
        return chunk_fill(chunk, size, strat);
    return (NULL);
}

t_mem_chunk     *chunk_fill(t_mem_chunk *chunk, size_t size, t_expstrat strat)
{
    chunk->user_size = size;
    if (strat & FILL_LARGE)
        chunk->prev_size = chunk->real_size;// (chunk->real_size >= size * 2 + sizeof(t_mem_chunk)) ? size * 2 + sizeof(t_mem_chunk) : chunk->real_size;
    else
        chunk->prev_size = chunk->user_size;
    chunk->status = USED;
    arena_alloc_delta(chunk->arena, chunk->user_size);
    DEBUG_ALLOC(chunk);
    return (chunk);
}

t_mem_chunk     *chunk_append(t_mem_chunk *chunk, size_t size, t_expstrat strat)
{
    DEBUG_LINE();    
    t_mem_chunk *new;
    if (strat & ALTERNATELY)
        new = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->user_size);
    else
        new = (t_mem_chunk *)((size_t)(chunk + 1) + chunk->prev_size);
    new->real_size = chunk->real_size - (((size_t)new - (size_t)chunk));//+ sizeof(t_mem_chunk));
    chunk->real_size -= new->real_size + sizeof(t_mem_chunk);
    new->prev = chunk;
    new->arena = chunk->arena;
    if (chunk->next)
        chunk->next->prev = new;    
    new->next = chunk->next;
    chunk->next = new;
    return chunk_fill(new, size, strat & APPEND_LARGE ? FILL_LARGE : FILL);
}

/*
**  Free chunk and try to expande itself to next if it's free
**  and join itself with right if exist
*/
int            arena_free_chunk(t_mem_chunk *chunk)
{
    DEBUG_LINE();    
    if (chunk->status != USED)
        return (0);
    chunk->status = FREE;
    arena_alloc_delta(chunk->arena, -chunk->user_size);
    chunk_try_join_next(chunk);
    DEBUG_ALLOC(chunk);
    chunk_try_join_prev(chunk);
    if (chunk->arena->buffer_used == 0)
        return (-1);
    return (1);
}

void            chunk_try_join_next(t_mem_chunk *chunk)
{
    DEBUG_LINE();    
    if (!chunk->next || chunk->next->status == USED)
        return ;
    chunk->real_size += chunk->next->real_size + sizeof(t_mem_chunk);
    chunk->next = chunk->next->next;
    if (chunk->next)
        chunk->next->prev = chunk;
}

void            chunk_try_join_prev(t_mem_chunk *chunk)
{
    DEBUG_LINE();    
    if (!chunk->prev)
    {        
        return ;
    }
    chunk->prev->real_size += chunk->real_size + sizeof(t_mem_chunk);
    chunk->prev->next = chunk->next;
    if (chunk->next)
        chunk->next->prev = chunk->prev;
}

t_mem_chunk     *arena_expande_chunk(t_mem_chunk *chunk, size_t size)
{
    DEBUG_LINE();
    size = SIZE_ALIGN(size, MEM_ARENA_AL);
    if (chunk->real_size >= size) {
        DEBUG_LINE();    
        chunk->user_size = size;
        if (chunk->user_size > chunk->prev_size)
            chunk->prev_size = chunk->user_size;
        return (chunk);
    }
    if (chunk->prev && chunk->prev->status == FREE && chunk->prev->real_size + chunk->real_size + sizeof(t_mem_chunk) >= size)
    {
        DEBUG_LINE();        
        chunk->prev->real_size += chunk->real_size + sizeof(t_mem_chunk);
        chunk->prev->status = USED;
        chunk->prev->next = chunk->next;
        if (chunk->next)
            chunk->next->prev = chunk->prev;
        memmove(chunk->prev + 1, chunk + 1, chunk->user_size);
        return (chunk);
    }
    return  (NULL);
}