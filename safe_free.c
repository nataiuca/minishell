/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:09:55 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 15:30:20 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void safe_free_vector(char **split) {
    int i = -1;
    while (split[++i])
        free(split[i]);
    if (split)
        free(split);
}

void safe_free_vector_elem(char **split) {
    int i = -1;
    while (split[++i]) {
        printf("freeing %s\n", split[i]);
        free(split[i]);
    }
}

void safe_free_minishell(t_minishell *minishell) {
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