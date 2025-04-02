/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:14:24 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/16 13:10:08 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcountchr(char *str, char chr)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (str[i] == chr)
			count++;
	}
	return (count);
}

/*
* Function to clean input param from spaces
* ' ' '\t' '\n' '\v' '\f' '\r'
* Return the cleaned string
*/
char	*trim_space_char(char *input)
{
	int		index;
	char	*tmp;
	char	*spaces;

	index = 0;
	spaces = " \t\n\v\f\r";
	while ((ft_isspace(input[0]) || ft_isspace(input[ft_strlen(input) - 1]))
		&& index <= 5)
	{
		tmp = ft_strtrim(input, &spaces[index]);
		free(input);
		input = tmp;
		index++;
	}
	return (input);
}

// Function to replace a string with another string
// the first value is the main string
// the second value is the string to be find and replaced
// the third value is the string to replace
char *ft_strreplace(char *str, char *old, char *new)
{
	char *result;
	int i, count = 0;
	int newlen = ft_strlen(new);
	int oldlen = ft_strlen(old);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; str[i] != '\0'; i++)
	{
		if (ft_strstr(&str[i], old) == &str[i])
		{
			count++;
			// Jumping to index after the old word.
			i += oldlen - 1;
		}
	}

	// Making new string of enough length
	result = (char *)safe_malloc(i + count * (newlen - oldlen) + 1);

	i = 0;
	while (*str)
	{
		// compare the substring with the result
		if (ft_strstr(str, old) == str)
		{
			strcpy(&result[i], new);
			i += newlen;
			str += oldlen;
		}
		else
			result[i++] = *str++;
	}
	result[i] = '\0';
	return (result);
}

// Function to valid if a string has the open and close of a quote or double quote
// Return true if the string is valid
bool valid_quotes(char *input, char quote)
{
	int i;
	int count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == quote)
			count++;
	}
	if (count % 2 == 0)
		return (true);
	return (false);
}
