/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:31:57 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 16:16:18 by jgamarra         ###   ########.fr       */
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

  // printf("parsepipe->ps: %s\n", *ps);
  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
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
