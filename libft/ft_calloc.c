/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 10:41:48 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/23 17:19:44 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*elem;

	elem = malloc(size * nmemb);
	if (!elem)
		return (0);
	ft_memset(elem, 0, size * nmemb);
	return (elem);
}
