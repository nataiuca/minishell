/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:09:29 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/17 22:04:43 by jgamarra         ###   ########.fr       */
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
	// load envp and save it in minishell->env allocanting memory
	i = 0;
	while (envp[i])
		i++;
	minishell->env = safe_malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		minishell->env[i] = ft_strdup(envp[i]);
	minishell->env[i] = NULL;
	
	// minishell->env = envp;
}

char *get_env_value(t_minishell *minishell, char *key)
{
    int len = ft_strlen(key);
    for (int i = 0; minishell->env[i]; i++) {
        if (ft_strncmp(minishell->env[i], key, len) == 0 && minishell->env[i][len] == '=') {
            return &minishell->env[i][len + 1];
        }
    }
    return NULL;
}

void set_env_value(t_minishell *minishell, char *key, char *value)
{
    int len = ft_strlen(key);
    for (int i = 0; minishell->env[i]; i++) {
        if (ft_strncmp(minishell->env[i], key, len) == 0 && minishell->env[i][len] == '=') {
            free(minishell->env[i]);

            // Allocate space for "key=value\0"
            int total_len = len + 1 + ft_strlen(value) + 1;
            minishell->env[i] = safe_malloc(total_len);

            // Manually build the string: key + '=' + value
            strcpy(minishell->env[i], key);
            strcat(minishell->env[i], "=");
            strcat(minishell->env[i], value);
            break ;
        }
    }
}
