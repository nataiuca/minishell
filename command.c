/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/31 00:08:45 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd *execcmd()
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd *)cmd;
}

struct cmd *parseexec(char **ps, char *es, t_minishell *minishell) {
    char *q, *eq;
    int tok, argc;
    struct execcmd *cmd;
    struct cmd *ret;

    ret = execcmd();
    cmd = (struct execcmd *)ret;

    argc = 0;

    while (!peek(ps, es, "|")) { 
        if ((tok = gettoken(ps, es, &q, &eq)) == 0)
            break;
        if (tok != 'a')
            panic("syntax");
        
        int len = eq - q;
        if ((q[0] == '\'' || q[0] == '"') && q[len - 1] == q[0]) {
            cmd->argv[argc] = strip_quotes(q, len);
        } else {
            cmd->argv[argc] = malloc(len + 1);
            if (!cmd->argv[argc])
                error_exit("Error: malloc failed.\n");
            ft_memcpy(cmd->argv[argc], q, len);
            cmd->argv[argc][len] = '\0';
        }
		{
			char *expanded = expand_token(cmd->argv[argc], minishell);
			free(cmd->argv[argc]);
			cmd->argv[argc] = expanded;
		}

        cmd->eargv[argc] = eq;
		{
            int pos = 0;
            while (cmd->argv[argc] && cmd->argv[argc][pos])
            {
                if (cmd->argv[argc][pos] == '$')
                    expand_variable((t_cmd *)cmd, argc, &pos, minishell);
                else
                    pos++;
            }
        }
        argc++;

        if (argc >= MAXARGS)
            panic("too many args");
        ret = parseredirs(ret, ps, es);
    }

    cmd->argv[argc] = 0;
    cmd->eargv[argc] = 0;
    return ret;
}

void valid_command(t_execcmd *ecmd, t_minishell *minishell)
{
  if (strchr("./", ecmd->argv[0][0]))
  {
    // #valid if command is executable
    if (access(ecmd->argv[0], F_OK) == -1)
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
  else
  {
    // loop into minishell->list_path to find the command path
    int i = -1;
    while (minishell->path_env[++i])
    {
      char *command = ft_strjoin("/", ecmd->argv[0]);
      char *full_command = ft_strjoin(minishell->path_env[i], command);
      if (access(full_command, F_OK) == 0)
      {
        ecmd->argv[0] = full_command;
        break;
      }
      free(command);
      free(full_command);
    }
    if (!strchr("./", ecmd->argv[0][0]))
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
}

struct cmd *parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);                 // es = end of string
  cmd = parseline(&s, es); // s = start of all the string
  peek(&s, es, "");
  if (s != es)
  {
    ft_putstr_fd("leftovers: ", 2);
    ft_putstr_fd(s, 2);
    ft_putchar_fd('\n', 2);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

// Execute cmd.  Never returns.
void runcmd(struct cmd *cmd, t_minishell *minishell)
{
    int p[2];
    struct execcmd *ecmd;
    struct redircmd *rcmd;
    struct pipecmd *pcmd;
    pid_t pid1, pid2;

    if (cmd == 0)
        exit(0);

    switch (cmd->type)
    {
        default:
            panic("runcmd: unknown type");
        case EXEC:
            ecmd = (struct execcmd *)cmd;
            if (ecmd->argv[0] == 0)
                exit(0);
            /* Puedes agregar comprobaciones de builtins si se requieren, o pasar directamente a exec */
            exec_command(ecmd->argv[0], ecmd->argv);
            perror("exec failed");
            exit(127);
            break;

			case REDIR:
			rcmd = (struct redircmd *)cmd;
			if (rcmd->hdoc) {
				int pipefd[2];
				pid_t pid;
				if (pipe(pipefd) < 0)
					panic("pipe error");
				if ((pid = fork1()) == 0) {
					close(pipefd[0]);
					write(pipefd[1], rcmd->hdoc, ft_strlen(rcmd->hdoc));
					close(pipefd[1]);
					exit(0);
				}
				waitpid(pid, NULL, 0);
				close(pipefd[1]);
				dup2(pipefd[0], STDIN_FILENO);
				close(pipefd[0]);
			} else {
				int fd;
				if (rcmd->mode == O_RDONLY) {
					fd = open(rcmd->file, O_RDONLY);
					if (fd < 0) {
						perror("open redirection failed");
						exit(1);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
				else if (rcmd->mode == (O_CREAT | O_WRONLY | O_TRUNC)) {
					fd = open(rcmd->file, O_CREAT | O_WRONLY | O_TRUNC, rcmd->right);
					if (fd < 0) {
						perror("open redirection failed");
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
				else if (rcmd->mode == (O_CREAT | O_WRONLY | O_APPEND)) {
					fd = open(rcmd->file, O_CREAT | O_WRONLY | O_APPEND, rcmd->right);
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
			runcmd(rcmd->cmd, minishell);
			break;		

        case PIPE:
            pcmd = (struct pipecmd *)cmd;
            if (pipe(p) < 0)
                panic("pipe error");
            if ((pid1 = fork1()) == 0)
            {
                dup2(p[1], STDOUT_FILENO);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->left, minishell);
                exit(0);
            }
            if ((pid2 = fork1()) == 0)
            {
                dup2(p[0], STDIN_FILENO);
                close(p[0]);
                close(p[1]);
                runcmd(pcmd->right, minishell);
                exit(0);
            }
            close(p[0]);
            close(p[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            break;
    }
    exit(0);
}

void exec_command(char *command, char **args)
{
  if (execvp(command, args) == -1)
  {
    perror("exec failed");
    exit(EXIT_FAILURE);
  }
}

void control_cmd(t_cmd *cmd, t_minishell *minishell)
{
	if (valid_builtins(cmd))
	{
		// cmd = prepare_builtins(cmd, minishell);
		run_internal(cmd, minishell);
	}
	else
	{
		run_external(cmd, minishell);
	}
}
