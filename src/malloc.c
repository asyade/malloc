/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 02:59:18 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/13 18:57:47 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libr.h"

void			*realloc(void *ptr, size_t size)
{
	void		*new;
	int			res;
	size_t		old;

	if (DEBUG_FNCS)
		ft_putfmt(DEBUG_PREFIX"\trealloc(%x, %u)\n", ptr, size);
	take_lock();
	if (ptr != NULL && size == 0)
		return (free_lock(ptr));
	else if (ptr == NULL || main_called(0) == 0)
		return (free_lock(mmemalloc_alloc(size > 0 ? size : MIN_ALLOC_SIZE)));
	if ((res = mmemalloc_expande(ptr, size)) == 1)
		return (free_lock(ptr));
	free_lock(NULL);
	if (res < 0)
		return (NULL);
	old = ((t_memmagic *)ptr - 1)->size - (2 * sizeof(t_memmagic));
	if ((new = malloc(size)) == NULL)
		return (NULL);
	if (new == ptr)
		return (new);
	ft_memcpy(new, ptr, old < size ? old : size);
	free(ptr);
	return (new);
}

void			*reallocf(void *ptr, size_t size)
{
	void		*new;

	if ((new = realloc(ptr, size)) == NULL)
	{
		free(ptr);
		return (NULL);
	}
	return (new);
}

void			*calloc(size_t nmemb, size_t size)
{
	void		*ptr;

	if ((ptr = malloc(nmemb * size)) == NULL)
		return (NULL);
	if (nmemb * size == 0)
		ft_memset(ptr, 0, 128);
	else
		ft_memset(ptr, 0, size * nmemb);
	return (ptr);
}

void			*valloc(size_t size)
{
	return (malloc(size));
}

void			*reallocarray(void *ptr, size_t nmemb, size_t size)
{
	return (realloc(ptr, nmemb * size));
}
