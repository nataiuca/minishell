/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:29:04 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/13 00:17:33 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, mode_t right, int fd, char *hdoc)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->right = right;
  cmd->fd = fd;
  cmd->hdoc = hdoc;
  return (struct cmd*)cmd;
}

char* process_heredoc(char *q, char *eq) {
    char *heredoc = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *input = stdin; // Usamos stdin para simular la entrada del usuario.

    // Inicializa una cadena vacía.
    heredoc = malloc(1);
    *heredoc = '\0';

    printf("Ingrese líneas para el heredoc, termine con '%.*s':\n", (int)(eq-q), q);
    while ((nread = getline(&line, &len, input)) != -1) {
        // Si la línea es el delimitador, termina.
        if (strncmp(line, q, eq-q) == 0 && line[eq-q] == '\n') {
            break;
        }
        // Concatenar la línea leída a la cadena heredoc.
        heredoc = realloc(heredoc, strlen(heredoc) + nread + 1);
        strcat(heredoc, line);
    }

    // Liberar memoria.
    free(line);

    return heredoc;
}

struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es) {
    int tok;
    char *q, *eq;
    char *hdoc = NULL;

    while(peek(ps, es, "<>")) {
        tok = gettoken(ps, es, 0, 0);
        if(tok == HDOC) {
            if(gettoken(ps, es, &q, &eq) != 'a')
                panic("missing delimiter for heredoc");
            hdoc = process_heredoc(q, eq);
            cmd = redircmd(cmd, NULL, NULL, 0, 0, 0, hdoc);
        } 
        else {
            if(gettoken(ps, es, &q, &eq) != 'a')
                panic("missing file for redirection");
            switch(tok) {
                case '<':
                    cmd = redircmd(cmd, q, eq, O_RDONLY, 0, 0, NULL);
                    break;
                case '>':
                    cmd = redircmd(cmd, q, eq, O_CREAT|O_WRONLY|O_TRUNC, 0644, 1, NULL);
                    break;
                case '+':  // >>, append
                    cmd = redircmd(cmd, q, eq, O_CREAT|O_WRONLY|O_APPEND, 0644, 1, NULL);
                    break;
            }
        }
    }
    return cmd;
}
