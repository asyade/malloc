/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddfront.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acorbeau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 02:48:09 by acorbeau          #+#    #+#             */
/*   Updated: 2017/07/05 02:48:11 by acorbeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstaddfront(t_list **root, t_list *new)
{
	t_list	*elem;

	if (!*root)
	{
		*root = new;
		return ;
	}
	elem = *root;
	while (elem->next)
		elem = elem->next;
	elem->next = new;
}
