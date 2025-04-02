/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/01 00:30:43 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
    t_execcmd *ecmd;
    int idx;
    int pos;
    char quote = 0;

    ecmd = (t_execcmd *)cmd;
    
    // Por ejemplo, usando ft_strcmp para los builtins (puedes usar ft_strstr)
    if (ft_strcmp(ecmd->argv[0], "echo") == 0)
    {
        /* Implementación de echo (tu código actual) */
        int new_line = 1;
        idx = 1;
        while (ecmd->argv[idx] && ft_strstr(ecmd->argv[idx], "-n"))
        {
            new_line = 0;
            idx++;
        }
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
					write(1, &ecmd->argv[idx][pos], 1);  // Imprimir comilla de apertura
					pos++;
					continue;
				}
				else if (quote && ecmd->argv[idx][pos] == quote)
				{
					write(1, &ecmd->argv[idx][pos], 1);  // Imprimir comilla de cierre
					quote = 0;
					pos++;
					continue;
				}

                if (quote != '\'' && ecmd->argv[idx][pos] == '$')
                {
                    pos++;
                    if (ecmd->argv[idx][pos] == '?')
                    {
                        char *status_str = ft_itoa(minishell->status);
                        write(1, status_str, ft_strlen(status_str));
                        free(status_str);
                        pos++;
                        continue;
                    }
                    else if ((ft_isalnum(ecmd->argv[idx][pos]) || ecmd->argv[idx][pos] == '_')
                             && ecmd->argv[idx][pos] != '\\' && ecmd->argv[idx][pos] != '"')
                    {
                        expand_variable(cmd, idx, &pos, minishell);
                        continue;
                    }
                    else
                    {
                        write(1, &ecmd->argv[idx][pos - 1], 1);
                        if (ecmd->argv[idx][pos] == '\\' || ecmd->argv[idx][pos] == '"')
                            continue;
                        else
                        {
                            write(1, &ecmd->argv[idx][pos], 1);
                            pos++;
                            continue;
                        }
                    }
                }
                if (ecmd->argv[idx][pos] == '\\')
                {
                    pos++;
                    continue;
                }
                write(1, &ecmd->argv[idx][pos], 1);
                pos++;
            }
            if (ecmd->argv[idx + 1])
                write(1, " ", 1);
            idx++;
        }
        if (new_line)
            write(1, "\n", 1);
        minishell->status = 0;
    }
    else if (ft_strcmp(ecmd->argv[0], "pwd") == 0)
    {
        char *pwd = getcwd(NULL, 0);
        if (!pwd)
        {
            printf("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
            minishell->status = 1;
            return;
        }
        write(1, pwd, ft_strlen(pwd));
        write(1, "\n", 1);
        free(pwd);
        minishell->status = 0;
    }
    else if (ft_strcmp(ecmd->argv[0], "cd") == 0)
    {
        /* Implementación de cd (tu código para cd) */
        // [...]
    }
    else if (ft_strcmp(ecmd->argv[0], "env") == 0)
    {
        int i = 0;
        while (minishell->env[i])
        {
            write(1, minishell->env[i], ft_strlen(minishell->env[i]));
            write(1, "\n", 1);
            i++;
        }
    }
    else if (ft_strcmp(ecmd->argv[0], "exit") == 0)
    {
        exit(minishell->status);
    }
    else if (ft_strcmp(ecmd->argv[0], "export") == 0)
    {
        /* Implementación de export, si corresponde */
    }
    else if (ft_strcmp(ecmd->argv[0], "unset") == 0)
    {
        /* Implementación de unset, si corresponde */
    }
    else if (ft_strcmp(ecmd->argv[0], "history") == 0)
	{
		/* Si se pasa "-c", se limpia el historial y se actualiza el archivo persistente */
		if (ecmd->argv[1] && ft_strcmp(ecmd->argv[1], "-c") == 0)
		{
			history_clear(minishell->history);
			save_history_file(minishell->history, ".minishell_history", 500);
			minishell->history_disabled = 1;  // Opcional: evitar agregar nuevos comandos esta sesión
			printf("Historial borrado.\n");
		}
		/* Si se pasa una opción tipo "-N" */
		else if (ecmd->argv[1] && ecmd->argv[1][0] == '-' &&
				ft_isdigit((unsigned char)ecmd->argv[1][1]))
		{
			history_print(minishell->history, ecmd->argv[1]);
		}
		else
		{
			history_print(minishell->history, NULL);
		}
		minishell->status = 0;
	}
}


void run_external(t_cmd *cmd, t_minishell *minishell)
{
	int status;
	pid_t pid;

	if((pid = fork1()) == 0)
	{
		runcmd(cmd, minishell);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		minishell->status = WEXITSTATUS(status);
	}
}
