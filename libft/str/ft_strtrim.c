/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:21:21 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 16:21:22 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	ft_isinset(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		start;
	int		end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen((char *)s1) - 1;
	while (ft_isinset(s1[start], set) == 1 && s1[start] != '\0')
		start++;
	while (ft_isinset(s1[end], set) == 1 && end > 0)
		end--;
	end = end - start + 1;
	if (end < 0)
		end = 0;
	str = malloc(end + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, (char *)s1 + start, end + 1);
	return (str);
}
