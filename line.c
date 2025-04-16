/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:10:48 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 13:25:17 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* nulterminate(struct cmd *cmd) {
    int i;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;
    if (cmd == 0)
        return 0;
    switch (cmd->type) {
        case EXEC:
            ecmd = (struct execcmd*)cmd;
            for (i = 0; ecmd->argv[i]; i++)
                *ecmd->eargv[i] = 0;
            break;
        case REDIR:
            rcmd = (struct redircmd*)cmd;
            nulterminate(rcmd->cmd);
            *rcmd->efile = 0;
            break;
        case PIPE:
            pcmd = (struct pipecmd*)cmd;
            nulterminate(pcmd->left);
            nulterminate(pcmd->right);
            break;
    }
    return cmd;
}

int fork1(void) {
    int pid = fork();
    if (pid == -1)
        panic("fork");
    return pid;
}


void panic(char *s) {
    ft_exit_message_fd(2, s, 1);
}