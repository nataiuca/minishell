/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:54:05 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 12:31:13 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_rep_char(char c, int fill)
{
	char	*str_fill;
	int		size;
	int		i;

	size = fill;
	str_fill = (char *)malloc(sizeof(char) * size);
	if (!str_fill)
		return (-1);
	i = 0;
	while (i < size)
	{
		str_fill[i] = c;
		i++;
	}
	str_fill[i] = 0;
	size = ft_putstr_size(str_fill, 1);
	free(str_fill);
	if (size == -1)
		return (size);
	return (fill);
}

int	get_length(char **str)
{
	int	num;

	num = (**str - '0');
	(*str)++;
	while (ft_isdigit(**str))
	{
		num = (num * 10) + (**str - '0');
		(*str)++;
	}
	return (num);
}

int	valid_length(int *size, int width, char c)
{
	if (width > *size)
	{
		if (print_rep_char(c, width - *size) == -1)
			return (-1);
		*size = width;
	}
	return (0);
}
