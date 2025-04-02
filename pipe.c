/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:31:57 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/02 21:04:48 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd* parsepipe(char **ps, char *es)
{
    struct cmd *cmd;
    t_minishell minishell;

    // Parsea el comando de la izquierda
    cmd = parseexec(ps, es, &minishell);
	if (((struct execcmd *)cmd)->argv[0] == NULL) {
        printf("parse error near `|'\n");
        exit(1);
    }

    // Si se encuentra un pipe, procesamos el comando de la derecha
    if (peek(ps, es, "|")) {
        // Consumir el token pipe
        gettoken(ps, es, 0, 0);

        // Verificar que después del pipe exista un comando válido.
        // Si se encuentra otro '|' o se llegó al final de la entrada, es un error.
        if (!**ps || peek(ps, es, "|")) {
            printf("parse error near `|'\n");
            exit(1);
        }

        // Parsea el comando de la derecha (recursivamente, lo que permite múltiples pipes)
        struct cmd *right = parsepipe(ps, es);
        if (right == NULL) {
            printf("parse error near `|'\n");
            exit(1);
        }
        cmd = pipecmd(cmd, right);
    }
    return cmd;
}

struct cmd* parseline(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parsepipe(ps, es);
//   while(peek(ps, es, "&")){
//     gettoken(ps, es, 0, 0);
//     cmd = backcmd(cmd);
//   }
//   if(peek(ps, es, ";")){
//     gettoken(ps, es, 0, 0);
//     cmd = listcmd(cmd, parseline(ps, es));
//   }
  return cmd;
}
