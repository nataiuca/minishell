/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:03:23 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/14 16:03:23 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*rslt;
	int		i;

	i = 0;
	rslt = (char *)malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (rslt == NULL)
	{
		return (NULL);
	}
	while (*s1 != '\0')
	{
		rslt[i] = *s1;
		i++;
		s1++;
	}
	rslt[i] = '\0';
	return (rslt);
}
