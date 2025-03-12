/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 11:43:00 by jgamarra          #+#    #+#             */
/*   Updated: 2025/02/08 23:04:44 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	valid_inital_param(int argc, char **envp, t_minishell *minishell)
{
	if (argc != 1)
		ft_exit_message("ERROR: Params are not available.", 1);
	init_env(minishell, envp);
}
