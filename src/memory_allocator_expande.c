/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_allocator_fn3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 17:00:20 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/09 05:21:51 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"

#define MAGOFF(allocator, magic)((size_t)magic - ALLOC_SPTR(allocator))

size_t			join_magics(t_memalloc *a, t_memmagic *magics[2])
{
	size_t		offset[2];
	size_t		drained_hi;
	size_t		curr_hi;

	offset[0] = MAGOFF(a, magics[0]);
	offset[1] = MAGOFF(a, magics[1]);
	//ft_putfmt("Before union : %u-%u-%u-%u\n", offset[0], magics[0]->size + offset[0], offset[1], magics[1]->size + offset[1]);
	if ((drained_hi = bheap_find(a->empty_entries, &(t_mementry){0, magics[1]}, 0)) == BH_NOTFOUND)
		return (BH_NOTFOUND);
	if ((curr_hi = bheap_find(a->used_entries, &(t_mementry){0, magics[0]}, 0)) == BH_NOTFOUND)
		return (BH_NOTFOUND);
	if (bheap_remove(a->empty_entries, drained_hi) != 0)
		return (BH_NOTFOUND);
	if (fill_mem_magic(a, offset[0], magics[0]->size + magics[1]->size, USED, 1) != 0)
		return (BH_NOTFOUND);
	//ft_putfmt("After union : %u-%u\n", offset[0], magics[0]->size + offset[0]);
	return (curr_hi);
}

int				memalloc_extrude(t_memalloc *a, t_memmagic *prev, size_t prev_index, size_t prev_size)
{
	size_t		prev_offset;
	size_t		new_sz;

	prev_offset = MAGOFF(a, prev);
	if (prev->size - prev_size < MIN_ALLOC_SIZE)
		return (0);
	new_sz = prev->size - prev_size;
	if (fill_mem_magic(a, prev_offset, prev_size, USED, 0) != 0)
		return (E_MAGIC);
	prev->size = prev_size;
	(USED_PTR(a) + prev_index)->size = prev_size;
	if (fill_mem_magic(a, prev_offset + prev_size, new_sz, FREE, 1) != 0)
		return (E_MAGIC);
	if (bheap_insert(a->empty_entries, &(t_mementry){new_sz, (t_memmagic *)((size_t)prev + prev->size)}) == BH_NOTFOUND)
		return (E_INS_EMPTY);
	return (0);
}

int				memalloc_try_expande(t_memalloc *a, void *addr, size_t ns)
{
	t_memmagic	*magics[2];
	size_t		begin;
	size_t		index;

	magics[0] = (t_memmagic *)addr - 1;
	begin = MAGOFF(a, magics[0]);
	ns = SIZE_ALIGN(ns + (2 * sizeof(t_memmagic)));
	if (magics[0]->size >= ns)
		return (1);
	if (begin + magics[0]->size >= a->buffer_size)
		return (0);
	if (a->range.min == (size_t)-1)
	{
		//ft_putfmt("Big heap can't be expanded !\n");
		return (0);
	}
	magics[1] = (t_memmagic *)((size_t)magics[0] + magics[0]->size);
	if (magics[0]->size + magics[1]->size > a->buffer_size)
		return (-1);
	if (magics[0]->size + magics[1]->size < ns || magics[1]->status == USED)
		return (0);
	if ((index = join_magics(a, magics)) == BH_NOTFOUND)
		return (E_UNDEF);
	(USED_PTR(a) + index)->size = magics[0]->size;
	return (memalloc_extrude(a, magics[0], index, ns) >= 0 ? 1 : -1);
}
