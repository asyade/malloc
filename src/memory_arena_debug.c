#include "memory_arena.h"
#include "malloc.h"

static int  get_errstd()
{
    return (2);        
}

void        dump_chk(t_mem_chunk *chk)
{
    while (chk)
    {
        ft_putstr_fd("0x", get_errstd());
        ft_putnbrbase_fd((size_t)(chk+1)  & 0x000FFFFF, "0123456789ABCDEF", get_errstd());
        ft_putstr_fd(" - 0x", get_errstd());
        ft_putnbrbase_fd(((size_t)(chk+1) + chk->user_size)  & 0x000FFFFF, "0123456789ABCDEF", get_errstd());
        ft_putstr_fd(" : ", get_errstd());
        ft_putnbrbase_fd(chk->user_size, "0123456789", get_errstd());
        ft_putendl_fd("", get_errstd());            
        chk = chk->next;
    }
}

void        dump_heap(t_mem_arena *arena, char *title)
{
    t_mem_chunk *chk;

    while (arena)
    {
        ft_putstr_fd(title, get_errstd());        
        ft_putnbrbase_fd((size_t)arena & 0x000FFFFF, "0123456789ABCDEF", get_errstd());
        ft_putendl_fd("", get_errstd());
        dump_chk(arena->buffer);        
        arena = arena->next;
        if (arena && arena->status & ROOT)
            break ;
    }
}

void        dump_arena(t_arena *arena)
{
    dump_heap(arena->small, "TINY : 0x");
    dump_heap(arena->medium, "SMALL : 0x");
    dump_heap(arena->big, "LARGE : 0x");
}

void        debugline(char *fname, int nbr, char *fn)
{
    int fd = get_errstd();
    
    ft_putstr_fd(fname, fd);
    ft_putchar_fd(':', fd);
    ft_putnbr_fd(nbr, fd);
    ft_putstr_fd(" in fn ", fd);
    ft_putstr_fd(fn, fd);
    ft_putstr_fd("\n", fd);
}

void        debugalloc(t_mem_chunk *chk)
{
    int fd = get_errstd();

    ft_putnbrbase_fd((size_t)chk, "0123456789ABCDEF", fd);
    ft_putstr_fd(" PREV ", fd);
    ft_putnbrbase_fd((size_t)chk->prev, "0123456789ABCDEF", fd);    
    ft_putstr_fd(" NEXT ", fd);
    ft_putnbrbase_fd((size_t)chk->next, "0123456789ABCDEF", fd);    
    ft_putstr_fd(" USER ", fd);
    ft_putnbrbase_fd(chk->user_size, "0123456789", fd);
    ft_putstr_fd(" REAL ", fd);
    ft_putnbrbase_fd(chk->real_size, "0123456789", fd);
    ft_putendl_fd("", fd);
}

void        debugarena(t_mem_arena *arena)
{
    int fd = get_errstd();
    
    ft_putstr_fd("@ARENA ", fd);
    ft_putnbrbase_fd(arena->buffer_used, "0123456789", fd);
    ft_putstr_fd("/", fd);
    ft_putnbrbase_fd(arena->buffer_size, "0123456789", fd);
    ft_putstr_fd("\n", fd);
}