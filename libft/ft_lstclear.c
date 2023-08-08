/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:42:28 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/24 09:37:16 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*init;
	t_list	*tmp;

	init = *lst;
	while (init)
	{
		tmp = init->next;
		(*del)(init->content);
		free(init);
		init = tmp;
	}
	*lst = 0;
}
