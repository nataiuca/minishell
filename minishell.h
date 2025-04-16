/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:21:09 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/27 17:00:18 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "minishell$ "
# define MAXARGS 10

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
#include <fcntl.h>  // Agrega esta línea para declarar O_RDWR
#include <string.h>

// LINUX
// # include <linux/limits.h> 
// MAC
# include <limits.h>
# include <stdbool.h>
# include <sys/types.h>

typedef enum e_type
{
	EXEC,
	REDIR,
	PIPE,
	HDOC,
}	t_type;

typedef struct cmd
{
  t_type type;
}	t_cmd;

typedef struct execcmd
{
	t_type type;
	char *argv[MAXARGS];
	char *eargv[MAXARGS];
}	t_execcmd;

typedef struct pipecmd
{
	int type;
	t_cmd *left;
	t_cmd *right;
}	t_pipecmd;

typedef struct redircmd
{
	int type;
	t_cmd *cmd;
	char *file;
	char *efile;
	int mode;
	mode_t right;
	int fd;
	char	*hdoc;
}	t_redircmd;

struct heredoccmd {
    t_type type;        // Debe ser HDOC
    struct cmd *cmd;    // Comando a ejecutar con la entrada heredoc
    char *delim;        // Delimitador, por ejemplo "EOF"
};

typedef struct s_history
{
    char **entries;
    int count;
    int capacity;
} t_history;

typedef enum e_response_msg
{
	CMD_NOT_FOUND,
	NO_SUCH_FILE,
	PERM_DENIED,
	AMBIGUOUS,
	TOO_MANY_ARGS,
	NUMERIC_REQUI
}	t_response_msg;

typedef enum e_response_code
{
	SUCCESS,
	GENERAL,
	CANT_EXEC = 126,
	NOT_FOUND,
	EXEC_255 = 255
}	t_response_code;

typedef struct s_response
{
	t_response_code	no;
	t_response_msg	msg;
	char		*cause;
}	t_response;

typedef struct minishell
{
	char	**path_env;
	char	**env;
	t_history *history;
	int	status;
}	t_minishell;

// env
void		init_env(t_minishell *minishell, char **envp);
int			ft_env(char **envp, char *key, char *value);
char *strip_quotes(char *start, char *end);

//params
void	valid_inital_param(int argc, char **envp, t_minishell *minishell);
int ft_isspace(int c);
void ft_exit_message_fd(int fd, const char *s, int code);
void ft_exit_message(const char *s, int code);

//safe func
void	*safe_malloc(size_t size);
pid_t	safe_fork(void);
int safe_open(char *file, int flags, mode_t mode);
void safe_pipe(int *pipefd);

// void	safe_execve(t_minishell *minishell, char **argv);

// interactive
void	catch_signal(void);
void	catch_interactive(t_history *history, char *input, char *prompt);
void	save_history(char *input);
char	*check_input_valid(char *input);

// safe_free.c
void	safe_free_vector(char **split);
void	safe_free_minishell(t_minishell *minishell);
void history_free(t_history *hist);
void free_exec_argv(struct execcmd *cmd);

// str_util
int ft_strcountchr(char *str, char chr);
char	*trim_space_char(char *input);
char **ft_copy_split(char **split);
char *ft_strreplace(char *str, char *old, char *new);
bool valid_quotes(char *input, char quote);

// token.c
int	gettoken(char **ps, char *es, char **q, char **eq);
int	peek(char **ps, char *es, char *toks);
void error_exit(const char *msg);

//expand.c
char *extract_token(char **ps, char *es);
char *extract_single_quotes(char **ps, char *es); 
char *extract_double_quotes(char **ps, char *es);

// command.c
struct cmd* execcmd(void);
// En mininshell.h
void runcmd(struct cmd *cmd, t_minishell *minishell);
void exec_command(char *command, char **args);

//parse.c
struct cmd* parseexec(char **ps, char *es);
struct cmd* parsecmd(char *s);

// redirection.c
struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, mode_t right, int fd, char *hdoc);
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es);

// pipe.c
struct cmd* pipecmd(struct cmd *left, struct cmd *right);
struct cmd* parsepipe(char **ps, char *es);
struct cmd* parseline(char **ps, char *es);

// line.c
struct cmd* nulterminate(struct cmd *cmd);
void panic(char *s);
int fork1(void);

// history.c
t_history *history_create(void);
void history_add(t_history *hist, const char *entry);
static char *construct_history_path(const char *histfile_name);
void load_history_file(t_history *hist, const char *histfile_name);
void save_history_file(t_history *hist, const char *histfile_name, int max_entries);

//history2.c
void history_print(t_history *hist, const char *option);
void history_clear(t_history *hist);

//vector.c
char **ft_vector_add_first(char **vector, char *new);
void ft_vector_remove_last_element(char **argv);
void ft_vector_trim(char **argv);
int ft_vector_size(char **split);
void replace_element_index(char **split, int index, char *tmp);

//cmd_impl.c
void run_internal(t_cmd *cmd, t_minishell *minishell);
void run_external(t_cmd *cmd, t_minishell *minishell);
int is_valid_quote(t_cmd *cmd, t_minishell *minishell);

//cmd_util.c
char *expand_variable(const char *input);

#endif
