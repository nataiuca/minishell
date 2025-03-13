/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/13 17:27:20 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd* parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

//   if(peek(ps, es, "("))
//     return parseblock(ps, es);
  // printf("ps: %s\n", *ps);
  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;")){ // loop character by character
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  // print_vector(cmd->argv);
  // print_vector(cmd->eargv);
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

struct cmd* parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s); // es = end of string
  cmd = parseline(&s, es); // s = start of all the string
  peek(&s, es, "");
  if(s != es){
    ft_putstr_fd("leftovers: ", 2);
    ft_putstr_fd(s, 2);
    ft_putchar_fd('\n', 2);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

// Execute cmd.  Never returns.
void runcmd(struct cmd *cmd) {
    int p[2];
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == 0)
        exit(0);

    switch (cmd->type)
    {
        case EXEC:
            ecmd = (struct execcmd *)cmd;
            if (ecmd->argv[0] == 0)
                exit(0);
            exec_command(ecmd->argv[0], ecmd->argv);
            // En caso de error:
            perror("exec failed");
            exit(127);
            break;

        case REDIR:
            rcmd = (struct redircmd *)cmd;
            if (rcmd->hdoc) {  
                // Implementación del here-document (<<)
                // Se crea un pipe para enviar por STDIN el contenido heredado.
                int pipefd[2];
                if (pipe(pipefd) < 0)
                    panic("pipe error");

                if (fork1() == 0) {
                    // En el proceso hijo se lee desde el input y se escribe en el pipe.
                    close(pipefd[0]); // Cerrar extremo de lectura.
                    char *line = NULL;
                    size_t len = 0;
                    ssize_t nread;
                    // Se lee línea a línea desde el input actual.
                    while (1) {
                        nread = getline(&line, &len, stdin);
                        if (nread < 0)
                            break;  // Error o EOF.
                        // Eliminar el salto de línea al final (si existe).
                        if (line[nread - 1] == '\n')
                            line[nread - 1] = '\0';
                        // Si la línea coincide **exactamente** con el delimitador, se termina.
                        if (strcmp(line, rcmd->hdoc) == 0)
                            break;
                        // Escribimos la línea en el pipe, agregando el salto de línea de vuelta.
                        write(pipefd[1], line, strlen(line));
                        write(pipefd[1], "\n", 1);
                    }
                    free(line);
                    close(pipefd[1]);
                    exit(0);
                }
                // En el proceso padre se redirige STDIN desde el pipe.
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
            else {  
                // Redirección normal (<, >, >>)
                int fd;
                if (rcmd->mode == O_RDONLY) {  
                    // Redirección de entrada "<"
                    fd = open(rcmd->file, O_RDONLY);
                    if (fd < 0) {
                        perror("open redirection failed");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
                else if (rcmd->mode == (O_WRONLY | O_CREAT | O_TRUNC)) {  
                    // Redirección de salida ">" (sobreescribe)
                    fd = open(rcmd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        perror("open redirection failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else if (rcmd->mode == (O_WRONLY | O_CREAT | O_APPEND)) {  
                    // Redirección de salida ">>" (append)
                    fd = open(rcmd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd < 0) {
                        perror("open redirection failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else {
                    perror("unknown redirection mode");
                    exit(1);
                }
            }
            // Se continúa con la ejecución del comando que sigue a la redirección.
            runcmd(rcmd->cmd);
            break;

        case PIPE:
            pcmd = (struct pipecmd *)cmd;
            if (pipe(p) < 0)
                panic("pipe error");

            if (fork1() == 0) {
                // Proceso hijo para la izquierda del pipe: redirige STDOUT al pipe.
                close(STDOUT_FILENO);
                dup2(p[1], STDOUT_FILENO);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->left);
            }

            if (fork1() == 0) {
                // Proceso hijo para la derecha del pipe: redirige STDIN desde el pipe.
                close(STDIN_FILENO);
                dup2(p[0], STDIN_FILENO);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->right);
            }

            close(p[0]);
            close(p[1]);
            wait(0);
            wait(0);
            break;

        default:
            panic("runcmd: unknown type");
    }
    exit(0);
}

void exec_command(char *command, char **args)
{
	// ft_printf("%s\n", command);
	// print_vector(args);
    if (execvp(command, args) == -1) {
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
}
