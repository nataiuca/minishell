/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:10:18 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:41:22 by natferna         ###   ########.fr       */
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

// En mininshell.h
void runcmd(struct cmd *cmd, t_minishell *minishell)
{
    int p[2];
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == 0)
        exit(0);

    switch (cmd->type) {
        case EXEC:
            ecmd = (struct execcmd *)cmd;
            if (ecmd->argv[0] == 0)
                exit(0);
            exec_command(ecmd->argv[0], ecmd->argv);
            perror("exec failed");
            exit(127);
            break;
        case REDIR:
            rcmd = (struct redircmd *)cmd;
            if (rcmd->hdoc) {
                int pipefd[2];
                if (pipe(pipefd) < 0)
                    panic("pipe error");
                if (fork1() == 0) {
                    close(pipefd[0]);
                    char *line = NULL;
                    while ((line = get_next_line(STDIN_FILENO))) {
                        size_t length = ft_strlen(line);
                        if (length > 0 && line[length - 1] == '\n')
                            line[length - 1] = '\0';
                        if (strcmp(line, rcmd->hdoc) == 0) {
                            free(line);
                            break;
                        }
                        write(pipefd[1], line, ft_strlen(line));
                        write(pipefd[1], "\n", 1);
                        free(line);
                    }
                    close(pipefd[1]);
                    exit(0);
                }
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
                } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_TRUNC)) {
                    fd = open(rcmd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        perror("open redirection failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_APPEND)) {
                    fd = open(rcmd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd < 0) {
                        perror("open redirection failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                } else {
                    perror("unknown redirection mode");
                    exit(1);
                }
            }
            runcmd(rcmd->cmd, minishell);
            break;
        case PIPE: {
            struct pipecmd *pcmd = (struct pipecmd *)cmd;
            int p[2];
            if (pipe(p) < 0) {
                perror("pipe error");
                exit(1);
            }
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
                runcmd(pcmd->left, minishell);
                exit(0);
            }
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
                runcmd(pcmd->right, minishell);
                exit(0);
            }
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

void exec_command(char *command, char **args) {
    if (execvp(command, args) == -1) {
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
}