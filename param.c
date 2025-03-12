/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 11:43:00 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/11 23:15:00 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ensure that three file descriptors are available (standard input, standard output, and standard error).
void valid_std_fd(void)
{
	int fd;
	
	fd = safe_open("/dev/tty", O_RDWR, 0);
	while(fd >= 0)
	{
		if (fd >= 3)
		{
			close(fd);
			break;
		}
		fd = safe_open("/dev/tty", O_RDWR, 0);
	}
}

void	valid_inital_param(int argc, char **envp, t_minishell *minishell)
{
	valid_std_fd();
	if (argc != 1)
		ft_exit_message("ERROR: Params are not available.", 1);
	init_env(minishell, envp);
}


