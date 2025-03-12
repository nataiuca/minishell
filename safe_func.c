/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 12:19:42 by jgamarra          #+#    #+#             */
/*   Updated: 2025/02/08 23:01:51 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*result;

	result = malloc(size);
	if (!result)
		ft_exit_message("Error: Malloc failed.\n", 1);
	return (result);
}

void	safe_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
		ft_exit_message("Error: Pipe failed.\n", 1);
}

static void	handle_fork_error(int status)
{
	if (EAGAIN == status)
		ft_exit_message("Error: The limit would be exceeded.\n", 1);
	else if (ENOMEM == status)
		ft_exit_message("Error: There is insufficient swap space.\n", 1);
	else if (-1 == status)
		ft_exit_message("Error: Fork failed.\n", 1);
}

pid_t	safe_fork(void)
{
	pid_t	pid;

	errno = 0;
	pid = fork();
	handle_fork_error(errno);
	return (pid);
}

int	safe_open(char *file, int flags, mode_t mode)
{
	int	fd;

	errno = 0;
	if (mode == 0)
		fd = open(file, flags);
	else
		fd = open(file, flags, mode);
	if (fd < 0)
	{
		printf("minishell: %s: %s\n", file, strerror(errno));
	}
	return (fd);
}

// void	safe_execve(t_minishell *minishell, char **argv)
// {
// 	errno = 0;
// 	if (execve(minishell->commands->args.path, argv, minishell->env_var) == -1)
// 		ft_exit_message(strerror(errno), 1);
// }
