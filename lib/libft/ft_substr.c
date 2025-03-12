/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:51:10 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/21 17:51:10 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*rslt;
	int		i;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (s_len < start)
		len = 0;
	else if (s_len < len || (s_len - start) < len)
		len = s_len - start;
	len = len + 1;
	rslt = (char *)malloc(len * sizeof(char));
	if (rslt == NULL)
		return (NULL);
	i = 0;
	while (len > 1 && *s + start)
	{
		rslt[i] = *(s + start);
		i++;
		start++;
		len--;
	}
	rslt[i] = '\0';
	return (rslt);
}
