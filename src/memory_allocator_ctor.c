/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_allocator_ctor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 16:53:03 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/08 20:29:24 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"
#include "malloc.h"

int				entries_cmp(void *aa, void *bb)
{
	t_mementry	*a;
	t_mementry	*b;

	a = (t_mementry *)aa;
	b = (t_mementry *)bb;
	if (a->addr == b->addr)
		return (0);
	return (a->addr < b->addr ? -1 : 1);
}

# define TMP_HEAP_SIZE ((sizeof(t_mementry) * 64) + (sizeof(t_bheap) * 2))

int				swap_heap(t_memalloc *alloc, t_bheap **heap)
{
	t_bheap		*new;
	size_t		sz;

	sz = SIZE_ALIGN((*heap)->buffer_size + sizeof(t_bheap) + (2 * STMM));
	if ((new = safe_memalloc_alloc(alloc, sz, 1)) == NULL)
		return (-1);
	*heap = ft_memcpy(new, *heap, (*heap)->buffer_size + sizeof(t_bheap));
	return (0);
}

t_memalloc		*make_heap_self_allocated(t_memalloc *alloc, size_t hs)
{
	t_bheap		*heaps[2];
	size_t		real;

	real = SIZE_ALIGN(hs + sizeof(t_bheap));
	heaps[0] = bheap_new(unaligned_memalloc_alloc(alloc, real), real - sizeof(t_bheap), STM, entries_cmp);
	heaps[1] = bheap_new(unaligned_memalloc_alloc(alloc, real), real - sizeof(t_bheap), STM, entries_cmp);
	heaps[0]->size = AUE(alloc)->size;
	heaps[1]->size = AEE(alloc)->size;
	ft_memcpy(heaps[0] + 1, AUE(alloc) + 1, AUE(alloc)->buffer_size);
	ft_memcpy(heaps[1] + 1, AEE(alloc) + 1, AEE(alloc)->buffer_size);
	AUE(alloc) = heaps[0];
	AEE(alloc) = heaps[1];
	return (alloc);
}

t_memalloc		*memalloc_new(size_t bs, size_t hs, t_szrange r)
{
	t_memalloc	*alloc;
	t_memchunk	*chunk;
	uint8_t		*buffer[TMP_HEAP_SIZE];
	

	if ((chunk = mchunk_alloc(sizeof(t_memalloc) + bs + ((hs + sizeof(t_bheap)) * 2))) == NULL)
		return (NULL);
	alloc = (t_memalloc *)(chunk + 1);
	alloc->buffer_size = chunk->size - (sizeof(t_memalloc));
	alloc->range = r;
	AEE(alloc) = bheap_new(buffer, TMP_HEAP_SIZE / 2, STM, entries_cmp);
	AUE(alloc) = bheap_new(buffer + (TMP_HEAP_SIZE / 2), TMP_HEAP_SIZE / 2, STM, entries_cmp);
	bheap_insert(AEE(alloc), MENT(alloc->buffer_size, alloc + 1));
	fill_mem_magic(alloc, 0, alloc->buffer_size, FREE, 1);
	return (make_heap_self_allocated(alloc, hs));
}

t_memalloc		*memalloc_new_noheap(size_t bs)
{
	t_memalloc	*alloc;
	t_memchunk	*chunk;
	
	bs *= 1.2;
	bs += 2	* sizeof(t_memmagic);
	if ((chunk = mchunk_alloc(bs + sizeof(t_memalloc))) == NULL)
		return (NULL);
	alloc = (t_memalloc *)(chunk + 1);
	alloc->buffer_size = chunk->size - (sizeof(t_memalloc));
	alloc->range = (t_szrange){-1, -1};
	AEE(alloc) = NULL;
	AUE(alloc) = NULL;
	fill_mem_magic(alloc, 0, alloc->buffer_size, USED, 1);
	return (alloc);
}

void			memalloc_destroy(t_memalloc *a)
{
	if (!a)
		return ;
	if (DEBUG_PAGE_DEL)
		ft_putfmt(DEBUG_PREFIX"\tDeleting %x\n", a);
	if (mchunk_free((t_memchunk *)((size_t)a - sizeof(t_memchunk))) != 0)
	{
		ft_putfmt("Can't unmap allocator, memory\
criticaly corupted, exit\n");
		exit(1);
	}
}
