/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 11:34:38 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 12:32:56 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_str(int width, char *val, char c)
{
	int		size;

	size = 0;
	if (!val)
		return (ft_putstr_size("(null)", 1));
	size = ft_strlen(val);
	if (valid_length(&size, width, c) == -1)
		return (-1);
	if (ft_putstr_size(val, 1) == -1)
		return (-1);
	return (size);
}

int	print_chr(int width, int val, char c)
{
	int	size;

	size = 1;
	if (valid_length(&size, width, c) == -1)
		return (-1);
	if (ft_putchar_size(val, 1) == -1)
		return (-1);
	return (size);
}

int	print_porcentage(int width, char c)
{
	int	size;

	size = 1;
	if (valid_length(&size, width, c) == -1)
		return (-1);
	if (ft_putchar_size('%', 1) == -1)
		return (-1);
	return (size);
}
