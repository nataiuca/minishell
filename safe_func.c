/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:09:33 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 13:26:11 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *safe_malloc(size_t size) {
    void *result = malloc(size);
    if (!result)
        ft_exit_message("Error: Malloc failed.\n", 1);
    return result;
}

void safe_pipe(int *pipefd) {
    if (pipe(pipefd) == -1)
        ft_exit_message("Error: Pipe failed.\n", 1);
}

int safe_open(char *file, int flags, mode_t mode) {
    int fd;
    errno = 0;
    if (mode == 0)
        fd = open(file, flags);
    else
        fd = open(file, flags, mode);
    if (fd < 0)
        printf("minishell: %s: %s\n", file, strerror(errno));
    return fd;
}

int safe_fork(void) {
    int pid = fork();
    if (pid < 0)
        ft_exit_message("Error: Fork failed.\n", 1);
    return pid;
}
