/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   managed_memory_allocator_heap.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 18:23:57 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/13 18:57:04 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"
#include "malloc.h"

void				*find_and_alloc(size_t sz, size_t index)
{
	t_bheap			*heap;
	t_memalloc		*node;
	void			*ptr;
	size_t			tmp;

	heap = mmemalloc_heap();
	if (index >= heap->size)
		return (NULL);
	tmp = ((size_t)(heap + 1)) + (index * sizeof(t_allocator));
	node = ((t_allocator *)tmp)->allocator;
	if (node->range.min < sz && node->range.max > sz &&
			node->empty_entries->size > 0)
	{
		if ((ptr = safe_memalloc_alloc(node, sz, 1)) != NULL)
			return (ptr);
	}
	if ((ptr = find_and_alloc(sz, BH_LEFT(index))) != NULL)
		return (ptr);
	if ((ptr = find_and_alloc(sz, BH_RIGHT(index))) != NULL)
		return (ptr);
	return (NULL);
}

t_memalloc			*memalloc_new_range(size_t range)
{
	if (range < 128)
	{
		small_page_count(1);
		return (memalloc_new(SMALL_SZ + sizeof(t_memalloc),
					4096, (t_szrange){SM_MIN, SM_MAX}));
	}
	if (range < 1024)
	{
		big_page_count(1);
		return (memalloc_new(BIG_SZ + sizeof(t_memalloc),
					4096, (t_szrange){BG_MIN, BG_MAX}));
	}
	return (memalloc_new(range + sizeof(t_memalloc),
							1024, (t_szrange){-1, -1}));
}

void				*insert_and_alloc(size_t range)
{
	t_memalloc		*allocator;

	if (mmemalloc_heap()->size >= MAX_ALLOC_PAGES)
		return (NULL);
	if ((allocator = memalloc_new_range(range)) == NULL)
		return (NULL);
	if (bheap_insert(mmemalloc_heap(), &(t_allocator){allocator}) == BHN)
		memalloc_panic(E_NOMEM);
	if (DEBUG_PAGE_INSERT)
		ft_putfmt(DEBUG_PREFIX"\tNew page insered (%x)\n\t\tsize"
					" : %u\n\t\trange : %u-%u\n",
					allocator, allocator->buffer_size,
					allocator->range.min, allocator->range.max);
	return (safe_memalloc_alloc(allocator, range, 0));
}

void				*mmemalloc_alloc(size_t size)
{
	void			*ptr;

	size = SIZE_ALIGN(size + (sizeof(t_memmagic) * 2));
	if (size > MAX_ALLOC_SIZE)
		return (NULL);
	if (size >= BG_MAX)
	{
		return (insert_and_alloc(size));
	}
	if ((ptr = find_and_alloc(size, 0)) != NULL)
		return (ptr);
	return (insert_and_alloc(size));
}
