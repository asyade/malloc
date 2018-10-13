/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_fn2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 03:08:19 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/13 18:59:08 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"
#include "malloc.h"
#include <pthread.h>

static PTMT		g_lock;

void			take_lock(void)
{
	static int	initialized = 0;

	if (!initialized)
	{
		initialized = 1;
		pthread_mutex_init(&g_lock, NULL);
	}
	pthread_mutex_lock(&g_lock);
}

void			*free_lock(void *ret)
{
	pthread_mutex_unlock(&g_lock);
	return (ret);
}

void			free(void *ptr)
{
	if (DEBUG_FNCS)
		ft_putfmt(DEBUG_PREFIX"\tfree(%x)\n", ptr);
	if (ptr == NULL)
		return ;
	take_lock();
	mmemalloc_free(ptr);
	free_lock(NULL);
}

void			*malloc(size_t size)
{
	void *ptr;

	if (DEBUG_FNCS)
		ft_putfmt(DEBUG_PREFIX"\tmalloc(%d)\n", size);
	take_lock();
	if (size < MIN_ALLOC_SIZE)
		size = MIN_ALLOC_SIZE;
	ptr = free_lock(mmemalloc_alloc(size));
	if ((size_t)ptr % 16)
	{
		ft_putfmt("ptr is not aligned !\n");
		exit(1);
	}
	return (ptr);
}
