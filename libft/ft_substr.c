/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 10:53:54 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/24 12:22:23 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*res;
	size_t			i;
	size_t			j;
	unsigned int	to_malloc;

	if (!s)
		return (0);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	to_malloc = ft_strlen(s + start);
	if (to_malloc > len)
		to_malloc = len;
	res = malloc((to_malloc + 1) * sizeof(char));
	if (!res)
		return (0);
	i = -1;
	j = 0;
	while (s[++i])
	{
		if (i >= start && j < len)
			res[j++] = s[i];
	}
	res[j] = '\0';
	return (res);
}
