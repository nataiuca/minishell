/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_controller.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:13:22 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 15:30:25 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_exec_argv(struct execcmd *cmd) {
    int i = 0;
    while (cmd->argv[i]) {
        free(cmd->argv[i]);
        i++;
    }
}

char **add_next_index_element(char **split, int index, char *tmp) {
    char **new;
    int i, j;
    i = -1;
    new = (char **)safe_malloc(sizeof(char *) * (ft_vector_size(split) + 2));
    while (split[++i] && i <= index)
        new[i] = split[i];
    new[i] = tmp;
    j = index;
    while (split[++j])
        new[++i] = split[j];
    new[i + 1] = NULL;
    return new;
}
