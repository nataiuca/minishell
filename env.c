/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:09:29 by jgamarra          #+#    #+#             */
/*   Updated: 2025/02/08 22:58:30 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* get PATH from envp and save it in minishell->list_path
*/
void	init_env(t_minishell *minishell, char **envp)
{
	int		i;
	char	**path;
	char	**path_values;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i], '=');
			path_values = ft_split(path[1], ':');
			safe_free_vector(path);
			break ;
		}
	}
	if (!path_values)
		ft_exit_message("Error: PATH not found in envp.\n", 1);
	minishell->path_env = path_values;
	minishell->env = envp;
}
