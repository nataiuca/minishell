/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:51:12 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/21 17:51:12 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			len;
	char			*rslt;
	unsigned int	i;

	len = ft_strlen(s);
	rslt = (char *)malloc(sizeof(char) * len + 1);
	if (rslt == NULL)
		return (NULL);
	i = 0;
	while (len > 0)
	{
		rslt[i] = (*f)(i, s[i]);
		len--;
		i++;
	}
	rslt[i] = '\0';
	return (rslt);
}
