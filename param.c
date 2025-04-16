/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:09:22 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 13:30:01 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void valid_std_fd(void) {
    int fd = safe_open("/dev/tty", O_RDWR, 0);
    while(fd >= 0) {
        if (fd >= 3) {
            close(fd);
            break;
        }
        fd = safe_open("/dev/tty", O_RDWR, 0);
    }
}

void valid_inital_param(int argc, char **envp, t_minishell *minishell) {
    valid_std_fd();
    if (argc != 1)
        ft_exit_message("ERROR: Params are not available.", 1);
    init_env(minishell, envp);
}

void ft_exit_message(const char *s, int code) {
    fprintf(stderr, "%s\n", s);
    exit(code);
}

void ft_exit_message_fd(int fd, const char *s, int code) {
    dprintf(fd, "%s\n", s);
    exit(code);
}

int ft_isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}