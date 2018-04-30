/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 02:41:53 by acorbeau          #+#    #+#             */
/*   Updated: 2017/07/05 02:41:56 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void assert_eq(int eq, int a,int b, char *str, int line)
{
    if ((eq && a == b) || (!eq && a != b))
        return ;
    ft_putstr("NOT ASSERT AT ");
    ft_putstr(str);
    ft_putstr(":");
    ft_putnbr(line);
    ft_putchar('\n');
}

size_t	ft_wstrlen(wchar_t *str)
{
	size_t i;

	i = 0;
	while (*str)
	{
		if (*str <= 0x7F)
			i++;
		else if (*str <= 0x7FF)
			i += 2;
		else if (*str <= 0xFFFF)
			i += 3;
		else if (*str <= 0x10FFFF)
			i += 4;
		str++;
	}
	return (i);
}
