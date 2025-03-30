/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:30:08 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 20:39:56 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_free_vector_elem(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
	{
		printf("freeing %s\n", split[i]);
		free(split[i]);
	}
}

/*
* Function to add a new elemento to the vector.
* Return the new vector.
*/
char	**ft_vector_add_first(char **vector, char *new)
{
	int		i;
	char	**new_vector;

	i = 0;
	while (vector[i])
		i++;
	new_vector = (char **)safe_malloc(sizeof(char *) * (i + 2));
	new_vector[0] = new;
	i = 0;
	while (vector[i])
	{
		new_vector[i + 1] = vector[i];
		i++;
	}
	new_vector[i + 1] = NULL;
	vector = new_vector;
	return (vector);
}

/*
* Function to remove the last element of the vector.
*/
void	ft_vector_remove_last_element(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	free(argv[i - 1]);
	argv[i - 1] = NULL;
}

/*
* Function to clean right and left white space of each element of the vector.
*/
void	ft_vector_trim(char **argv)
{
	int		i;
	char	*tmp;

	i = -1;
	while (argv[++i])
	{
		tmp = ft_strtrim(argv[i], " ");
		free(argv[i]);
		argv[i] = tmp;
	}
}

int ft_vector_size(char **split)
{
	int i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void replace_element_index(char **split, int index, char *tmp)
{
	free(split[index]);
	split[index] = tmp;
}

/*
* Function to add a new element next to the provided index.
* Return the new vector.
*/
char	**add_next_index_element(char **split, int index, char *tmp)
{
	char	**new;
	int i;
	int j;

	i = -1;
	new = (char **)safe_malloc(sizeof(char *) * (ft_vector_size(split) + 2));
	while (split[++i] && i <= index)
		new[i] = split[i];
	new[i] = tmp;
	j = index;
	while (split[++j])
		new[++i] = split[j];
	return (new);
}

/*
* Function to remove quotes from the element.
*/
void remove_quotes(char **split, char quote)
{
	int i;
	int j;
	char *tmp;

	i = -1;
	while (split[++i])
	{
		if (ft_strchr(split[i], quote))
		{
			tmp = ft_strtrim(split[i], &quote);
			replace_element_index(split, i, tmp);
		}
	}
}

void	print_vector(char **vector)
{
	int	i;

	i = 0;
	while (vector[i])
	{
		printf("vector[%d]: %s\n", i, vector[i]);
		i++;
	}
}
