/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skipquotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:20:10 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 16:20:11 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	count_quotes(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (*str != 0)
	{
		if (*str == '\"')
			i++;
		if (*str == '\'')
			j++;
		str++;
	}
	if (i % 2 != 0 || j % 2 != 0)
		return (-1);
	return (1);
}

void	ft_skip(char *str)
{
	int	i;
	int	j;

	if (count_quotes(str) == -1)
	{
		ft_putstr_fd("Odd number of quotes!\n", 2);
		exit(EXIT_FAILURE);
	}
	i = 0;
	j = 0;
	while (str[i] != 0)
	{
		if (str[i] == '"' || str[i] == '\'')
			i++;
		else
		{
			str[j] = str[i];
			i++;
			j++;
		}
	}
	str[j] = 0;
}
