/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_allocator_expande.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 17:00:20 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/13 16:34:18 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"

size_t			join_magics(t_memalloc *a, t_memmagic *magics[2])
{
	size_t		offset[2];
	size_t		dhi;
	size_t		chi;

	offset[0] = MAGOFF(a, magics[0]);
	offset[1] = MAGOFF(a, magics[1]);
	if ((dhi = bheap_find(a->empty_entries, MENT(0, magics[1]), 0)) == BHN)
		return (BH_NOTFOUND);
	if ((chi = bheap_find(a->used_entries, MENT(0, magics[0]), 0)) == BHN)
		return (BH_NOTFOUND);
	if (bheap_remove(a->empty_entries, dhi) != 0)
		return (BH_NOTFOUND);
	dhi = magics[0]->size + magics[1]->size;
	if (fill_mem_magic(a, offset[0], dhi, USED, 1) != 0)
		return (BH_NOTFOUND);
	return (chi);
}

int				memalloc_extrude(TMA *a, TMM *prev, size_t pi, size_t ps)
{
	size_t		prev_offset;
	size_t		new_sz;

	prev_offset = MAGOFF(a, prev);
	if (prev->size - ps < MIN_ALLOC_SIZE)
		return (0);
	new_sz = prev->size - ps;
	if (fill_mem_magic(a, prev_offset, ps, USED, 0) != 0)
		return (E_MAGIC);
	prev->size = ps;
	(USED_PTR(a) + pi)->size = ps;
	if (fill_mem_magic(a, prev_offset + ps, new_sz, FREE, 1) != 0)
		return (E_MAGIC);
	if (bheap_insert(a->empty_entries,
						&(t_mementry){new_sz,
						(t_memmagic *)((size_t)prev + prev->size)}) == BHN)
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
		return (0);
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
