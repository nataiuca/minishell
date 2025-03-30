/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:35:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 21:25:49 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NUL-terminate all the counted strings.
struct cmd* nulterminate(struct cmd *cmd)
{
    int i;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == 0)
        return 0;

    switch (cmd->type) {
    case EXEC:
        ecmd = (struct execcmd*)cmd;
        for (i = 0; ecmd->argv[i]; i++) {
            if (ecmd->eargv[i]) // Check eargv[i] is not NULL
                *ecmd->eargv[i] = 0;
        }
        break;

    case REDIR:
        rcmd = (struct redircmd*)cmd;
        if (rcmd->cmd) // Check rcmd->cmd is not NULL
            nulterminate(rcmd->cmd);
        if (rcmd->efile) // Check rcmd->efile is not NULL
            *rcmd->efile = 0;
        break;

    case PIPE:
        pcmd = (struct pipecmd*)cmd;
        if (pcmd->left) // Check pcmd->left is not NULL
            nulterminate(pcmd->left);
        if (pcmd->right) // Check pcmd->right is not NULL
            nulterminate(pcmd->right);
        break;

    case HDOC:
        // Manejo específico para heredocs, si es necesario
        break;

    case ECHO:
        // Si cmd->type indica un comando "echo", puedes decidir qué hacer aquí
        break;

    case CD:
        // Similar a echo; puedes decidir si realizar algo en este caso
        break;

    default:
        // Captura cualquier caso no manejado explícitamente
        panic("Unhandled command type");
        break;
    }

    return cmd;
}

void panic(char *s)
{
  ft_exit_message_fd(2, s, 1);
}

int fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}
