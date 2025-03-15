/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:04:21 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 00:37:05 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_free_vector(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
	{
		free(split[i]);
	}
	if (split)
		free(split);
}

void	safe_free_minishell(t_minishell *minishell)
{
	if (minishell->path_env)
		safe_free_vector(minishell->path_env);
}

void history_free(t_history *hist)
{
    if (!hist)
        return;
    for (int i = 0; i < hist->count; i++)
        free(hist->entries[i]);
    free(hist->entries);
    free(hist);
}
