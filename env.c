/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:09:06 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 13:11:19 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_env(t_minishell *minishell, char **envp) {
    int i = -1;
    char **path = NULL;
    char **path_values = NULL;
    while (envp[++i]) {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0) {
            path = ft_split(envp[i], '=');
            path_values = ft_split(path[1], ':');
            safe_free_vector(path);
            break;
        }
    }
    if (!path_values)
        ft_exit_message("Error: PATH not found in envp.\n", 1);
    minishell->path_env = path_values;
    minishell->env = envp;
}

char *strip_quotes(char *start, char *end) {
    int len = end - start;
    char *result = malloc(len + 1);
    int ri = 0, i = 0;
    while (i < len) {
        if (start[i] != '\'' && start[i] != '"')
            result[ri++] = start[i];
        i++;
    }
    result[ri] = '\0';
    return result;
}