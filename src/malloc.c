/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 02:59:18 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/09 03:45:03 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libr.h"

void			*realloc(void *ptr, size_t size)
{
	void		*new;
	int			res;

	take_lock();
	if (ptr != NULL && size == 0)
	{
		ptr = mmemalloc_alloc(32);
		mmemalloc_free(ptr);
		return (free_lock(ptr));
	}
	else if (ptr == NULL)
		return (free_lock(mmemalloc_alloc(size > 0 ? size : 32)));
	if ((res = mmemalloc_expande(ptr, size)) == 1)
		return (free_lock(ptr));
	else if (res < 0)
	{
		ft_putfmt(CL_RED"ERROR, realloc return -1\n"CL_RESET);
		return (NULL);
	}
	else if ((new = mmemalloc_alloc(size)) == NULL)
		return (free_lock(NULL));
	free_lock(NULL);
	//ft_putfmt("save %p data into %p\n", ptr, new);
	ft_memcpy(new, ptr, ((t_memmagic *)ptr - 1)->size);
	//ft_putfmt("free %p replaced by %p\n", ptr, new);
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
	return (ptr);
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
