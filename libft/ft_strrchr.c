/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:33:50 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/23 18:04:07 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	if (s[i] == (unsigned char)c)
		return ((char *)&s[i]);
	while (--i >= 0)
	{
		if (s[i] == (unsigned char)c)
			return ((char *)&s[i]);
	}
	return (0);
}
