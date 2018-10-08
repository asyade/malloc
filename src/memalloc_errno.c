/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memalloc_errno.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <acorbeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 16:44:54 by acorbeau          #+#    #+#             */
/*   Updated: 2018/10/08 22:34:41 by acorbeau         ###   ########.fr       */
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

int				main_called(int called)
{
	static int i = 0;

	if (called)
		i = 1;
	return (i);
}

void __attribute__ ((constructor (999))) premain(void)
{
	main_called(1);
}

void			memalloc_panic(int code)
{
	if (main_called(0) == 0)
		return ;
	ft_putfmt("Memory arena corrupted exit, (error: %d)\n", code);
	raise(1);
}
