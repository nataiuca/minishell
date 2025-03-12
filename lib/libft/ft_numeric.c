/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numeric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 11:55:51 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 12:41:52 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	valid_number(int *size, int *val, int width, char c)
{
	if (*val < 0)
	{
		if (ft_putchar_size('-', 1) == -1)
			return (-1);
		*val = 1;
	}
	else
		*val = 0;
	if (valid_length(size, width, c) == -1)
		return (-1);
	return (0);
}

int	print_numeric(int width, int val, char c)
{
	char	*str_num;
	int		size;

	str_num = ft_itoa(val);
	if (!str_num)
		return (-1);
	size = ft_strlen(str_num);
	if (valid_number(&size, &val, width, c) == -1)
	{
		free(str_num);
		return (-1);
	}
	if (ft_putstr_size(str_num + val, 1) == -1)
	{
		free(str_num);
		return (-1);
	}
	free(str_num);
	return (size);
}

int	print_usignedint(int width, unsigned int val, char c)
{
	int				size;
	unsigned int	tmp;

	size = 0;
	tmp = val;
	while (tmp > 0)
	{
		tmp = tmp / 10;
		size++;
	}
	if (size == 0)
		size++;
	if (valid_length(&size, width, c) == -1)
		return (-1);
	if (ft_putunsignbr_fd(val, 1) == -1)
		return (-1);
	return (size);
}
