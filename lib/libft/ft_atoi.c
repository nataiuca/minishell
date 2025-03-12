/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:03:23 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/14 16:03:23 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ignore_char(char c)
{
	if (c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f' || c == ' ' || c == '\n'
		|| c == '-' || c == '+')
		return (1);
	else
		return (0);
}

static int	valid_sign(char c, int sign)
{
	if (sign != 0 && ignore_char(c))
	{
		sign = sign + sign;
	}
	else
	{
		if (c == '-')
			sign--;
		if (c == '+')
			sign++;
	}
	return (sign);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 0;
	while (*str != '\0')
	{
		sign = valid_sign(*str, sign);
		if (ft_isdigit(*str))
		{
			result = (result * 10) + (*str - '0');
			if (sign == 0)
				sign = 1;
		}
		else if (sign > 1 || sign < -1)
		{
			if (sign < 0)
				return (result * -1);
			return (result);
		}
		else if (!ignore_char(*str))
			return (result * sign);
		str++;
	}
	return (result * sign);
}
