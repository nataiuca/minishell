/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_controller.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:39:59 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 17:59:15 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int valid_builtins(t_cmd *cmd)
{
	t_execcmd *ecmd;
	ecmd = (t_execcmd *)cmd;
	
	return (ft_strstr(ecmd->argv[0], "echo")||
			ft_strstr(ecmd->argv[0], "cd")||
			ft_strstr(ecmd->argv[0], "pwd")||
			ft_strstr(ecmd->argv[0], "export")||
			ft_strstr(ecmd->argv[0], "unset")||
			ft_strstr(ecmd->argv[0], "env")||
			ft_strstr(ecmd->argv[0], "exit"))||
			ft_strcmp(ecmd->argv[0], "history") == 0;
}

t_cmd *prepare_builtins(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	int idx;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		// quote validator
		char quote=0;
		idx = 1;
		while (ecmd->argv[idx])
		{
			pos = 0;
			while (ecmd->argv[idx][pos])
			{
				if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
					quote = ecmd->argv[idx][pos];
				else if (quote && ecmd->argv[idx][pos] == quote)
					quote = 0;
				pos++;
			}
			idx++;
		}
		if (quote)
		{
			write(2, "Error: Unmatched quote\n", 23);
			return (cmd);
		}
		// quote validator
		
		// new line validator
		int new_line = 1;
		idx = 1;
		while (ecmd->argv[idx] && ft_strstr(ecmd->argv[idx], "-n"))
		{
			new_line = 0;
			idx++;
		}
		// new line validator

		// print echo
		if (!ecmd->argv[idx])
			write(1, "", 1);
		while (ecmd->argv[idx])
		{
			pos = 0;
			while (ecmd->argv[idx][pos])
			{
				if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
				{
					quote = ecmd->argv[idx][pos];
					pos++;
					continue ;
				}
				else if (quote && ecmd->argv[idx][pos] == quote)
				{
					quote = 0;
					pos++;
					continue ;
				}
				if (quote != '\'' && ecmd->argv[idx][pos] == '$' && ecmd->argv[idx][pos + 1] == '?')
				{
					write(1, ft_itoa(minishell->status), ft_strlen(ft_itoa(minishell->status)));
					pos += 2;
					continue ;
				}
				write(1, &ecmd->argv[idx][pos], 1);
				pos++;
			}
			// print if next element is not empty
			if (ecmd->argv[idx + 1])
				write(1, " ", 1);
			idx++;
		}
		if (new_line)
			write(1, "\n", 1);
		// print echo
	}
	return (cmd);
}

t_cmd *prepare_builtins_old(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	t_echocmd *echo;
	
	ecmd = (t_execcmd *)cmd;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		echo = (t_echocmd *)cmd;
		echo->type = ECHO;
		echo->new_line = false;
		echo->simple_quote = false;
		echo->double_quote = false;
		echo->invalid = false;
		
		int vectorlen = ft_vector_size(ecmd->argv);
		int vectorindex = 0;
		// remove extra -n TODO
		
		int index = 0;
		char quote;
		if (vectorlen > 1)
		{
			while (ecmd->argv[++vectorindex])
			{
				//valid if the element is "-n" and skip if it repeat in the next element
				if (ft_strstr(ecmd->argv[vectorindex], "-n"))
				{
					//change value to empty string
					while (ecmd->argv[vectorindex] && ft_strstr(ecmd->argv[vectorindex], "-n"))
					{
						ecmd->argv[vectorindex] = "";
						echo->new_line = true;
						vectorindex++;
					}
					if (ecmd->argv[vectorindex] != 0)
						continue ;
					else
						break ;
				}
				index = 0;
				while(ecmd->argv[vectorindex][index] && (ecmd->argv[vectorindex][index] == '\'' || ecmd->argv[vectorindex][index++] == '\"'))
					;
				quote = ecmd->argv[vectorindex][index++];
				if (quote == '\'' || quote == '\"')
				{
					while(ecmd->argv[vectorindex][index] && ecmd->argv[vectorindex][index++] == quote)
						;
					if (quote != ecmd->argv[vectorindex][index])
					{
						echo->invalid = true;
						printf("Error: Invalid quotes\n");
						minishell->status = 1;
						break ;
					}
				}
				
			}
		}
		else
		{
			ecmd->argv[1] = "";
		}
		
		// if (vectorlen > 1)
		// {
		// 	// valid quotes
		// 	print_vector(ecmd->argv);
		// 	while (ecmd->argv[++vectorindex])
		// 	{
		// 		if (ft_strchr(ecmd->argv[vectorindex], '\'') && !echo->double_quote)
		// 		{
		// 			if (valid_quotes(ecmd->argv[vectorindex], '\'') && !echo->double_quote)
		// 			{
		// 				ecmd->argv[vectorindex] = ft_strtrim(ecmd->argv[vectorindex], "\'");
		// 				echo->simple_quote = true;
		// 			}
		// 			else
		// 			{
		// 				printf("Error: Invalid quotes\n");
		// 				echo->invalid = true;
		// 				break ;
		// 			}
		// 		}
		// 		if (ft_strchr(ecmd->argv[vectorindex], '\"') && !echo->simple_quote)
		// 		{
		// 			if (valid_quotes(ecmd->argv[vectorindex], '\"') && !echo->simple_quote)
		// 			{
		// 				ecmd->argv[vectorindex] = ft_strtrim(ecmd->argv[vectorindex], "\"");
		// 				echo->double_quote = true;
		// 			}
		// 			else
		// 			{
		// 				printf("Error: Invalid quotes\n");
		// 				echo->invalid = true;
		// 				break ;
		// 			}
		// 		}
		// 		// valid symbol $?
		// 		if (ft_strstr(ecmd->argv[vectorindex], "$?") && echo->simple_quote == false)
		// 		{
		// 			// replace the character $? with a value
		// 			printf("replace character\n");
		// 			ecmd->argv[vectorindex] = ft_strreplace(ecmd->argv[vectorindex], "$?", ft_itoa(minishell->status));
		// 		}
		// 	}
		// }
		// // print_vector(ecmd->argv);
		return((t_cmd *)echo);
	}
	else if (ft_strstr(ecmd->argv[0], "cd"))
	{
		// ft_cd(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "pwd"))
	{
		// ft_pwd();
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "export"))
	{
		// ft_export(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "unset"))
	{
		// ft_unset(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "env"))
	{
		// ft_env(ecmd->minishell->env);
		exit(0);
	}
	// else if (ft_strstr(ecmd->argv[0], "exit"))
	// {
	// 	// ft_exit(ecmd->minishell->env);
	// 	exit(0);
	// }
	return (cmd);
}

