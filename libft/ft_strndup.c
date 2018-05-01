/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 02:42:56 by acorbeau          #+#    #+#             */
/*   Updated: 2017/07/05 02:42:58 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char	*ft_strndup(const char *s1, size_t size)
{
	int		i;
	char	*dst;

	if (s1 != NULL)
	{
		dst = NULL;
		dst = (char*)malloc(sizeof(char) * (size + 1));
		if (dst == NULL)
			return (NULL);
		i = -1;
		while (++i < (int)size)
			dst[i] = s1[i];
		dst[i] = '\0';
		return (dst);
	}
	return (NULL);
}
