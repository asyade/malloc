/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 02:48:19 by acorbeau          #+#    #+#             */
/*   Updated: 2017/07/05 02:48:21 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memdup(void *data, size_t len)
{
	void	*dup;

	if (!data || !(dup = malloc(sizeof(len))))
		return (NULL);
	ft_memcpy(dup, data, len);
	return (dup);
}
