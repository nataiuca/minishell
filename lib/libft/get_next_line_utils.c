/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 23:01:10 by jgamarra          #+#    #+#             */
/*   Updated: 2024/08/31 18:35:08 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc_zero(size_t count, size_t size)
{
	void	*result;
	size_t	n;
	size_t	index;

	result = malloc(count * size);
	if (result == NULL)
	{
		return (NULL);
	}
	n = count * size;
	index = 0;
	while (index < n)
	{
		((char *)result)[index] = '\0';
		index++;
	}
	return (result);
}

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	i = 0;
	while (*s != '\0')
	{
		i++;
		s++;
	}
	return (i);
}

size_t	ft_strappend(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	idx;
	size_t	len;

	dst_len = ft_strlen_gnl(dst);
	src_len = ft_strlen_gnl(src);
	idx = 0;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	len = src_len + 1;
	while (len-- > 0)
	{
		dst[idx + dst_len] = src[idx];
		idx++;
	}
	return (dst_len + src_len);
}

char	*ft_strjoin_free(char *s1, char const *s2)
{
	size_t	len;
	char	*rslt;

	if (!s1)
		s1 = ft_calloc_zero(1, 1);
	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1;
	rslt = (char *)ft_calloc_zero(len, sizeof(char));
	if (rslt == NULL)
	{
		free(s1);
		return (NULL);
	}
	rslt[0] = 0;
	ft_strappend(rslt, s1, len);
	free(s1);
	ft_strappend(rslt, s2, len);
	return (rslt);
}

char	*gnl_substr(char const *s, unsigned int start, size_t len)
{
	char	*rslt;
	int		i;
	size_t	s_len;

	s_len = ft_strlen_gnl(s);
	if (s_len < start)
		len = 0;
	else if (s_len < len || (s_len - start) < len)
		len = s_len - start;
	len = len + 1;
	rslt = (char *)ft_calloc_zero(len, sizeof(char));
	if (rslt == NULL)
		return (NULL);
	i = 0;
	while ((len > 1) && (s[start]))
	{
		rslt[i] = s[start];
		i++;
		start++;
		len--;
	}
	rslt[i] = '\0';
	return (rslt);
}
