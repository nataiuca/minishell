/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:21:09 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/02 20:21:25 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "minishell$ "
# define MAXARGS 10

# include "lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
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
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}	t_type;

typedef struct s_history
{
    char **entries;
    int count;
    int capacity;
} t_history;

typedef struct minishell
{
	char	**path_env;
	char	**env;
	int		status;
	t_history *history;
	int     history_disabled;
}	t_minishell;

typedef struct echocmd
{
	int type;
	bool new_line;
	bool simple_quote;
	bool double_quote;
	bool invalid;
}	t_echocmd;

typedef struct cmd
{
	t_type type;
}	t_cmd;

typedef struct execcmd
{
	int type;
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
    t_type type;       
    struct cmd *cmd;   
    char *delim;        
};

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

// env
void		init_env(t_minishell *minishell, char **envp);
char *get_env_value(t_minishell *minishell, char *key);
void set_env_value(t_minishell *minishell, char *key, char *value);

//params
void	valid_inital_param(int argc, char **envp, t_minishell *minishell);
void prepare_minishell(t_minishell *minishell);

//safe func
void	*safe_malloc(size_t size);
pid_t	safe_fork(void);
int safe_open(char *file, int flags, mode_t mode);
void safe_pipe(int *pipefd);
// void	safe_execve(t_minishell *minishell, char **argv);

// interactive
void	catch_signal(void);
void	catch_interactive(char *input, char *prompt);
void	save_history(char *input);
char	*check_input_valid(char *input);

// safe_free.c
void	safe_free_vector(char **split);
void	safe_free_minishell(t_minishell *minishell);

// str_util
int ft_strcountchr(char *str, char chr);
char	*trim_space_char(char *input);
char *ft_strreplace(char *str, char *old, char *new);
bool valid_quotes(char *input, char quote);

// token.c
int	gettoken(char **ps, char *es, char **q, char **eq);
int	peek(char **ps, char *es, char *toks);
void error_exit(const char *msg);
char *strip_quotes(const char *token, int len);
int  tokenize_char(char c, char *quote);

// command.c
struct cmd* execcmd();
struct cmd* parseexec(char **ps, char *es, t_minishell *minishell);
struct cmd* parsecmd(char *s);
void runcmd(struct cmd *cmd, t_minishell *minishell);
void exec_command(char *command, char **args);
void control_cmd(t_cmd *cmd, t_minishell *minishell);

// redirection.c
struct cmd *redircmd(struct cmd *cmd, char *file, char *efile, int mode, mode_t right, int fd, char *hdoc);
char *process_heredoc(char *q, char *eq);
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es);


// pipe.c
struct cmd* pipecmd(struct cmd *left, struct cmd *right);
struct cmd* parsepipe(char **ps, char *es);
struct cmd* parseline(char **ps, char *es);

// line.c
struct cmd* nulterminate(struct cmd *cmd);
void panic(char *s);
int fork1(void);

// cmd_controller
int valid_builtins(t_cmd *cmd);

// cmd_impl
void run_internal(t_cmd *cmd, t_minishell *minishell);
void run_external(t_cmd *cmd, t_minishell *minishell);

// cmd_util.c
char *expand_token(const char *token, t_minishell *minishell);
void expand_variable(t_cmd *cmd, int idx, int *pos, t_minishell *minishell);
int is_valid_quote(t_cmd *cmd, t_minishell *minishell);
char *getenv_minishell(t_minishell *minishell, char *key);

// vector
int ft_vector_size(char **split);
void	print_vector(char **vector);
void	safe_free_vector_elem(char **split);
char	**ft_vector_add_first(char **vector, char *new);
void	ft_vector_remove_last_element(char **argv);
void	ft_vector_trim(char **argv);
void replace_element_index(char **split, int index, char *tmp);
char	**add_next_index_element(char **split, int index, char *tmp);

// history.c
t_history *history_create(void);
void history_add(t_history *hist, const char *entry);
char *construct_history_path(const char *histfile_name);
void load_history_file(t_history *hist, const char *histfile_name);
void save_history_file(t_history *hist, const char *histfile_name, int max_entries);

//history2.c
void history_print(t_history *hist, const char *option);
void history_clear(t_history *hist);
void history_free(t_history *hist);

void	print_vector(char **vector);

#endif
