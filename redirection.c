/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:29:04 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 22:10:03 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd *redircmd(struct cmd *cmd, char *file, char *efile, int mode, mode_t right, int fd, char *hdoc)
{
    struct redircmd *rcmd = malloc(sizeof(*rcmd));
    if (!rcmd)
        panic("malloc failed");
    memset(rcmd, 0, sizeof(*rcmd));
    rcmd->type = REDIR;
    rcmd->cmd = cmd;
    rcmd->file = file;
    rcmd->efile = efile;
    rcmd->mode = mode;
    rcmd->right = right;
    rcmd->fd = fd;
    rcmd->hdoc = hdoc;  // Asignamos el heredoc
    return (struct cmd *)rcmd;
}

struct cmd *parseredirs(struct cmd *cmd, char **ps, char *es)
{
    int tok;
    char *q, *eq;
    char *hdoc = NULL;

    while (peek(ps, es, "<>")) {
        tok = gettoken(ps, es, &q, &eq);
        /* Si queremos implementar heredoc, asumimos que HDOC es una constante
         * que identifica el token "<<" que hayas definido en gettoken(). */
        if (tok == HDOC) {  
            /* Se espera que después del token heredoc se encuentre el delimitador */
            if (gettoken(ps, es, &q, &eq) != 'a')
                panic("missing delimiter for heredoc");
            hdoc = process_heredoc(q, eq);  // Lee el heredoc (explicada más abajo)
            cmd = redircmd(cmd, NULL, NULL, 0, 0, 0, hdoc);
        } 
        else {  // Para los otros casos (<, >, >>)
            if (gettoken(ps, es, &q, &eq) != 'a')
                panic("missing file for redirection");
            switch (tok) {
                case '<':
                    cmd = redircmd(cmd, q, eq, O_RDONLY, 0, 0, NULL);
                    break;
                case '>':
                    cmd = redircmd(cmd, q, eq, O_CREAT | O_WRONLY | O_TRUNC, 0644, 1, NULL);
                    break;
                case '+':  // En tu caso, token '+' representa ">>"
                    cmd = redircmd(cmd, q, eq, O_CREAT | O_WRONLY | O_APPEND, 0644, 1, NULL);
                    break;
            }
        }
    }
    return cmd;
}

char *process_heredoc(char *q, char *eq) {
    char *heredoc = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *input = stdin;

    /* Extraer el delimitador entre q y eq */
    char delimiter[eq - q + 1];
    strncpy(delimiter, q, eq - q);
    delimiter[eq - q] = '\0';

   // printf("HEREDOC START: Delimiter is '%s'\n", delimiter);

    /* Inicializar buffer vacío para el heredoc */
    heredoc = malloc(1);
    if (!heredoc)
        panic("Memory allocation failed");
    *heredoc = '\0';

    while (1) {
        /* Imprimir el prompt en /dev/tty para que no se incluya en la entrada heredoc */
        int tty_fd = open("/dev/tty", O_WRONLY);
        if (tty_fd != -1) {
            write(tty_fd, "heredoc> ", 9);
            close(tty_fd);
        }
        
        nread = getline(&line, &len, input);
        if (nread == -1)
            break;
        
        /* Si la línea coincide EXACTAMENTE con el delimitador (sin espacios adicionales)
           y se termina en salto de línea, se finaliza la lectura */
        if (strncmp(line, delimiter, strlen(delimiter)) == 0 &&
            line[strlen(delimiter)] == '\n')
            break;

        heredoc = realloc(heredoc, strlen(heredoc) + nread + 1);
        if (!heredoc)
            panic("Memory allocation failed");
        strcat(heredoc, line);
    }

    free(line);
    return heredoc;
}
