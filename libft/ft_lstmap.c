/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:51:16 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/24 13:36:31 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin;
	t_list	*back;
	t_list	*tmp;

	if (!lst)
		return (0);
	begin = ft_lstnew((*f)(lst->content));
	if (!begin)
		return (0);
	lst = lst->next;
	back = begin;
	while (lst)
	{
		tmp = ft_lstnew((*f)(lst->content));
		if (!tmp)
		{
			ft_lstclear(&begin, (*del));
			return (0);
		}
		back->next = tmp;
		back = back->next;
		lst = lst->next;
	}
	return (begin);
}
