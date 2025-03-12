/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexadecimal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:42:45 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 15:57:22 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	len_number_hexa(unsigned long long val, int *size)
{
	if (val == 0)
		*size = 1;
	else
	{
		while (val > 0)
		{
			val = val / 16;
			*size += 1;
		}
	}
}

int	print_hexadecimal(char str, int width, unsigned long long val, char c)
{
	int	size;

	size = 0;
	len_number_hexa(val, &size);
	if (valid_length(&size, width, c) == -1)
		return (-1);
	if (convert_hexadecimal(val, str) == -1)
		return (-1);
	return (size);
}

int	convert_hexadecimal(unsigned long long val, char c)
{
	if (val >= 16)
	{
		if (convert_hexadecimal(val / 16, c) == -1)
			return (-1);
		if (convert_hexadecimal(val % 16, c) == -1)
			return (-1);
	}
	else
	{
		if (val < 10)
		{
			if (ft_putchar_size(val + '0', 1) == -1)
				return (-1);
		}
		else
		{
			if (ft_putchar_size(val - 10 + c, 1) == -1)
				return (-1);
		}
	}
	return (0);
}

static int	print_len_hex(int width, int *size, char c)
{
	*size += 2;
	if (c == ' ')
	{
		if (print_rep_char(c, width - *size) == -1)
			return (-1);
		if (ft_putstr_size("0x", 1) == -1)
			return (-1);
	}
	else
	{
		if (ft_putstr_size("0x", 1) == -1)
			return (-1);
		if (print_rep_char(c, width - *size) == -1)
			return (-1);
	}
	return (0);
}

int	print_pointer_dimension(int width, unsigned long long pointer, char c)
{
	int	size;

	size = 0;
	len_number_hexa(pointer, &size);
	if (width > size)
	{
		print_len_hex(width, &size, c);
		size += width - size;
	}
	else
	{
		if (ft_putstr_size("0x", 1) == -1)
			return (-1);
		size += 2;
	}
	if (convert_hexadecimal(pointer, 'a') == -1)
		return (-1);
	return (size);
}
