/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 00:04:18 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 10:15:58 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_size(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_putstr_size(char *s, int fd)
{
	return (write(fd, s, ft_strlen(s)));
}

int	ft_putnbr_size(int n, int fd)
{
	if (n == -2147483648)
		if (ft_putstr_size("-2147483648", fd) == -1)
			return (-1);
	if (n < 0)
	{
		if (ft_putchar_size('-', fd) == -1)
			return (-1);
		n *= -1;
	}
	if (n > 9)
	{
		if (ft_putnbr_size(n / 10, fd) == -1
			|| ft_putnbr_size(n % 10, fd) == -1)
			return (-1);
	}
	else if (n <= 9)
	{
		if (ft_putchar_size((char)n + '0', fd) == -1)
			return (-1);
	}
	return (0);
}

int	ft_putunsignbr_fd(unsigned int n, int fd)
{
	if (n > 9)
	{
		if (ft_putnbr_size(n / 10, fd) == -1)
			return (-1);
		if (ft_putnbr_size(n % 10, fd) == -1)
			return (-1);
	}
	else if (n <= 9)
	{
		if (ft_putchar_size((char)n + '0', 1) == -1)
			return (-1);
	}
	return (0);
}
