/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 17:49:49 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/21 17:51:06 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_split(char const *s, char c)
{
	int		split;
	size_t	w;

	split = 1;
	w = 0;
	while (*s)
	{
		if (*s != c && w == 0)
		{
			split++;
			w = 1;
		}
		else if (*s == c)
			w = 0;
		s++;
	}
	return (split);
}

static void	init_var(size_t *start, size_t *end, size_t *i, int *split)
{
	*start = 0;
	*end = 0;
	*i = 0;
	*split = 0;
}

static void	*free_list(char **s, int split)
{
	while (split >= 0)
	{
		free(s[split]);
		split--;
	}
	free(s);
	return (NULL);
}

static char	**set_word(char **rslt, char const *s, char c, size_t len)
{
	size_t	i;
	int		split;
	size_t	start;
	size_t	end;

	init_var(&start, &end, &i, &split);
	while (i <= len)
	{
		if (s[i] != c && s[i] != 0)
			end++;
		else if ((s[i] == c || s[i] == 0) && end > 0)
		{
			rslt[split] = ft_substr(s, start, end);
			if (!rslt[split])
				return (free_list(rslt, split));
			start = start + end;
			end = 0;
			split++;
		}
		if (s[i] == c)
			start++;
		i++;
	}
	rslt[split] = 0;
	return (rslt);
}

char	**ft_split(char const *s, char c)
{
	char	**rslt;

	rslt = (char **)malloc(sizeof(char *) * count_split(s, c));
	if (!rslt)
		return (NULL);
	rslt = set_word(rslt, s, c, ft_strlen(s));
	if (!rslt)
		return (NULL);
	return (rslt);
}
