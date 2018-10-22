/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/13 16:19:47 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/13 16:21:23 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"
#include "malloc.h"

void            show_allocator(t_memalloc *allocator)
{
    size_t      i;
    size_t      a;
    size_t      b;
    t_mementry  *e;

    i = 0;
    while (i < allocator->used_entries->size)
    {
        e = ((t_mementry *)BH_INDEX(allocator->used_entries, i));
        a = ((size_t)(e->addr + 1)) & SHM_MSK; 
        b = ((size_t)(e->addr + 1) + (e->size - (2 * STMM))) & SHM_MSK; 
        ft_putfmt("0x%x - 0x%x : %d octets\n", a, b, e->size - (2 * STMM));
        i++;
    }
}

void            show_tiny_alloc_mem(void *allocator)
{
	t_memalloc	*a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != SM_MAX)
        return ;
    ft_putfmt("TINY : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void			show_med_alloc_mem(void *allocator)
{
	t_memalloc	*a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != BG_MAX)
        return ;
    ft_putfmt("SMALL : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void			show_big_alloc_mem(void *allocator)
{
	t_memalloc	*a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max != 140737353822224)
        return ;
    ft_putfmt("LARGE : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void            show_undef_alloc_mem(void *allocator)
{
    t_memalloc *a;

    a = ((t_allocator *)allocator)->allocator;
    if (a->range.max == 140737353822224 ||
            a->range.max == SM_MAX ||
            a->range.max == BG_MAX)
        return ;
    ft_putfmt("UNDEF : 0x%x\n", (size_t)(a + 1) & SHM_MSK);
    show_allocator(a);
}

void			show_alloc_mem(void)
{
	t_bheap		*heap;

    heap = mmemalloc_heap();
    bheap_iter(heap, show_tiny_alloc_mem, 0);
    bheap_iter(heap, show_med_alloc_mem, 0);
    bheap_iter(heap, show_big_alloc_mem, 0);
    bheap_iter(heap, show_undef_alloc_mem, 0);
}
