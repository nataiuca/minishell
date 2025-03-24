/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:09:29 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/24 16:57:25 by natferna         ###   ########.fr       */
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

char *strip_quotes(char *start, char *end)
{
    int len = end - start;
    char *result = malloc(len + 1);
    int ri = 0;
    int i = 0;

    while (i < len) {
        // Eliminar las comillas simples o dobles
        if (start[i] != '\'' && start[i] != '"') {
            result[ri++] = start[i];
        }
        i++;
    }

    result[ri] = '\0';  // Asegurar terminación de cadena
    return result;
}
