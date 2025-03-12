/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:51:14 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/21 17:51:14 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*rslt;
	int		start;
	int		end;
	size_t	set_len;

	start = 0;
	set_len = ft_strlen(set);
	while (s1[start] && ft_memchr(set, s1[start], set_len) != NULL)
	{
		start++;
	}
	end = ft_strlen(s1);
	while (end > 0 && ft_memchr(set, s1[end - 1], set_len) != NULL)
	{
		end--;
	}
	rslt = ft_substr(s1, start, end - start);
	return (rslt);
}
