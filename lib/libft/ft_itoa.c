/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:51:16 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/21 17:51:16 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	init_rslt(char *rslt, size_t len, int sign, int n)
{
	rslt[len] = (n % 10) + '0';
	if ((len == 0) && (sign == -1))
		rslt[len] = 45;
}

char	*ft_itoa(int n)
{
	size_t	len;
	int		sign;
	char	*rslt;

	sign = 1;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len = ft_intlen(n);
	if (n < 0)
	{
		sign = -1;
		n = n * sign;
		len++;
	}
	rslt = (char *)malloc(sizeof(char) * (len + 1));
	if (rslt == NULL)
		return (NULL);
	rslt[len] = '\0';
	while (len-- > 0)
	{
		init_rslt(rslt, len, sign, n);
		n = n / 10;
	}
	return (rslt);
}
