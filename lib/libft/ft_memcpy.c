/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:03:22 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/14 16:03:22 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	char	*rslt;

	if (!dst && !src)
		return (NULL);
	rslt = dst;
	while (len > 0)
	{
		*(char *)dst = *(char *)src;
		dst++;
		src++;
		len--;
	}
	return ((char *)rslt);
}
