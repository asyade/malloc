/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   managed_memory_allocator_fn2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 18:24:04 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/08 20:28:32 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"

int					small_page_count(int delta)
{
	static int		nbr = 0;

	nbr += delta;
	return (nbr);
}

int					big_page_count(int delta)
{
	static int		nbr = 0;

	nbr += delta;
	return (nbr);
}

int					memallocator_cmpf(void *aa, void *bb)
{
	size_t			a;
	size_t			b;

	a = (size_t)((t_allocator *)aa)->allocator;
	b = (size_t)((t_allocator *)bb)->allocator;
	if (a == b)
		return (0);
	return (a < b) ? -1 : 1;
}

t_bheap				*mmemalloc_heap(void)
{
	static t_bheap	*heap = NULL;
	t_memchunk		*c;

	if (heap == NULL)
	{
		if ((c = mchunk_alloc(ALLOCATORS_HEAP_SIZE)) == NULL)
			memalloc_panic(E_NOMEM);
		heap = bheap_new(c + 1, c->size, sizeof(t_allocator), memallocator_cmpf);
	}
	return (heap);
}
