/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:49:45 by jgamarra          #+#    #+#             */
/*   Updated: 2024/06/27 21:54:30 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*rslt;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	rslt = (char *)malloc(len * sizeof(char));
	if (rslt == NULL)
		return (NULL);
	rslt[0] = 0;
	ft_strlcat(rslt, s1, len);
	ft_strlcat(rslt, s2, len);
	return (rslt);
}
