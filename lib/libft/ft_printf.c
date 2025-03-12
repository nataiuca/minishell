/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 15:46:31 by jgamarra          #+#    #+#             */
/*   Updated: 2024/04/21 16:04:45 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_printf.h"

static	int	ft_check_format(char **str, va_list *valist, int width, char c)
{
	int	size;

	size = 0;
	if (**str == 'c')
		size = print_chr(width, va_arg(*valist, int), c);
	else if (**str == 's')
		size = print_str(width, va_arg(*valist, char *), c);
	else if (**str == 'p')
		size = print_pointer_dimension(width,
				va_arg(*valist, unsigned long long), c);
	else if (**str == 'd' || **str == 'i')
		size = print_numeric(width, va_arg(*valist, int), c);
	else if (**str == 'u')
		size = print_usignedint(width, va_arg(*valist, unsigned int), c);
	else if (**str == 'x')
		size = print_hexadecimal('a', width, va_arg(*valist, unsigned int), c);
	else if (**str == 'X')
		size = print_hexadecimal('A', width, va_arg(*valist, unsigned int), c);
	else if (**str == '%')
		size = print_porcentage(width, c);
	else
		return (0);
	(*str)++;
	return (size);
}

static int	ft_check_flag(char **str, int *size, va_list *valist)
{
	int	status;

	status = 0;
	if (**str == '%')
	{
		(*str)++;
		if (**str == '0')
			status = ft_check_format(str, valist, get_length(str), '0');
		else if (ft_isdigit(**str))
			status = ft_check_format(str, valist, get_length(str), ' ');
		else
			status = ft_check_format(str, valist, 0, ' ');
		if (status == -1)
			return (status);
		else
			*size += status;
	}
	else
	{
		if (ft_putchar_size(**str, 1) == -1)
			return (-1);
		(*size)++;
		(*str)++;
	}
	return (0);
}

int	ft_printf(char const *s, ...)
{
	va_list	valist;
	char	*str;
	int		size;

	va_start(valist, s);
	str = (char *)s;
	size = 0;
	while (*str)
	{
		if (ft_check_flag(&str, &size, &valist) == -1)
			return (-1);
	}
	va_end(valist);
	return (size);
}
