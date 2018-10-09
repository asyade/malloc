/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memalloc_errno.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 16:44:54 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/09 05:07:26 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libr.h"
#include <signal.h>

int				errcode(int val, int set)
{
	static int	err = 0;

	if (set)
		err = val;
	return (err);
}

int				memalloc_geterr(void)
{
	return (errcode(0, 0));
}

int				memalloc_seterr(int code)
{
	return (errcode(code, 1));
}

void			premain(void)
{
	main_called(1);
}

void			dump_alloc_mem()
{
	
}

void			memalloc_panic(int code)
{
	if (main_called(0) == 0)
		return ;
	ft_putfmt("Memory arena corrupted exit, (error: %d)\n", code);
	exit(1);
	
}
