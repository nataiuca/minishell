/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:03:22 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/14 16:03:22 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*rslt;

	if (!dst && !src)
		return (NULL);
	rslt = dst;
	if (dst > src && dst < (src + len))
	{
		while (len-- > 0)
		{
			((char *)dst)[len] = ((char *)src)[len];
		}
	}
	else
	{
		while (len > 0)
		{
			*(char *)dst = *(char *)src;
			dst++;
			src++;
			len--;
		}
	}
	return (rslt);
}
