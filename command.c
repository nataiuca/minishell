/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/24 22:42:40 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  free_exec_argv((struct execcmd *)cmd);
  return (struct cmd*)cmd;
}

struct cmd* parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);

  while(!peek(ps, es, "|)&;")){ // loop character by character
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");

    // Verificar si hay comillas y procesar solo los tokens que lo necesiten
    if (q[0] == '"' || q[0] == '\'') {
        // Solo aplicar strip_quotes si es necesario
        cmd->argv[argc] = extract_token(&q, es);
    } else {
        cmd->argv[argc] = q;
    }

    cmd->eargv[argc] = eq;
    argc++;

    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }

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
					// En el proceso hijo se lee desde STDIN y se escribe en el pipe.
					close(pipefd[0]); // Cerrar extremo de lectura.
					char *line = NULL;
					// Se lee línea a línea desde el input:
					while ((line = get_next_line(STDIN_FILENO))) {
						// Usamos ft_strlen de tu libft para obtener la longitud.
						size_t length = ft_strlen(line);
						// Si la línea termina en salto de línea, lo eliminamos.
						if (length > 0 && line[length - 1] == '\n')
							line[length - 1] = '\0';
						// Si la línea coincide EXACTAMENTE con el delimitador, se termina.
						if (strcmp(line, rcmd->hdoc) == 0) {
							free(line);
							break;
						}
						// Escribimos la línea en el pipe, agregándole un salto de línea.
						write(pipefd[1], line, ft_strlen(line));
						write(pipefd[1], "\n", 1);
						free(line);
					}
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
			{
				struct pipecmd *pcmd = (struct pipecmd *)cmd;
				int p[2];
			
				if (pipe(p) < 0) {
					perror("pipe error");
					exit(1);
				}
			
				/* Proceso hijo izquierdo: redirige la salida estándar al extremo de escritura del pipe */
				pid_t pid_left = fork1();
				if (pid_left < 0) {
					perror("fork error (left)");
					exit(1);
				}
				if (pid_left == 0) {
					if (dup2(p[1], STDOUT_FILENO) < 0) {
						perror("dup2 error (left)");
						exit(1);
					}
					close(p[0]);
					close(p[1]);
					runcmd(pcmd->left);
					exit(0);
				}
			
				/* Proceso hijo derecho: redirige la entrada estándar desde el extremo de lectura del pipe */
				pid_t pid_right = fork1();
				if (pid_right < 0) {
					perror("fork error (right)");
					exit(1);
				}
				if (pid_right == 0) {
					if (dup2(p[0], STDIN_FILENO) < 0) {
						perror("dup2 error (right)");
						exit(1);
					}
					close(p[0]);
					close(p[1]);
					runcmd(pcmd->right);
					exit(0);
				}
			
				/* Proceso padre: cierra ambos extremos del pipe y espera a que finalicen los hijos */
				close(p[0]);
				close(p[1]);
				int status;
				waitpid(pid_left, &status, 0);
				waitpid(pid_right, &status, 0);
			}
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
