/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleseur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 11:52:16 by rleseur           #+#    #+#             */
/*   Updated: 2021/11/24 15:08:06 by rleseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char c, char const *set)
{
	int	i;

	i = -1;
	while (set[++i])
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

static int	len_final(char const *s1, char const *set,
		size_t *start, size_t *end)
{
	while (s1[*start] && is_charset(s1[*start], set))
		(*start)++;
	while (s1[*end] && is_charset(s1[*end], set))
		(*end)--;
	return (*end - *start);
}

static char	*str_malloc(void)
{
	char	*str;

	str = malloc(sizeof(char));
	if (!str)
		return (0);
	str[0] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	start;
	size_t	end;
	int		len;
	int		i;

	if (!s1 || !set)
		return (0);
	start = 0;
	end = ft_strlen(s1) - 1;
	len = len_final(s1, set, &start, &end) + 1;
	if (start == ft_strlen(s1))
		return (str_malloc());
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (0);
	i = -1;
	while (++i < len)
		str[i] = s1[start + i];
	str[i] = '\0';
	return (str);
}
