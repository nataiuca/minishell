/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:12:47 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:41:50 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
    t_execcmd *ecmd;
    int idx;
    int pos;

    ecmd = (t_execcmd *)cmd;

    // Lógica de manejo de comillas y caracteres especiales
    char quote = 0;
    if (ft_strstr(ecmd->argv[0], "echo"))
    {
        if (!is_valid_quote(cmd, minishell))
        {
            return;
        }

        // Verificación de la opción "-n" para no imprimir nueva línea
        int new_line = 1;
        idx = 1;
        while (ecmd->argv[idx] && ft_strstr(ecmd->argv[idx], "-n"))
        {
            new_line = 0;
            idx++;
        }

        // Impresión del comando echo
        if (!ecmd->argv[idx])
            write(1, "", 1);
        while (ecmd->argv[idx])
        {
            pos = 0;
            while (ecmd->argv[idx][pos])
            {
                // Manejo de comillas y expansión de variables
                if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
                {
                    quote = ecmd->argv[idx][pos];
                    pos++;
                    continue;
                }
                else if (quote && ecmd->argv[idx][pos] == quote)
                {
                    quote = 0;
                    pos++;
                    continue;
                }

                // Expansión de variables de entorno
                if (quote != '\'' && ecmd->argv[idx][pos] == '$')
                {
                    pos++;
                    if (ecmd->argv[idx][pos] == '?')
                    {
                        write(1, ft_itoa(minishell->status), ft_strlen(ft_itoa(minishell->status)));
                        pos++;
                        continue;
                    }
                    else
                    {
                        expand_variable(cmd, idx, &pos, minishell);
                        continue;
                    }
                }

                // Ignorar barras invertidas
                if (ecmd->argv[idx][pos] == '\\')
                {
                    pos++;
                    continue;
                }

                // Escribir el carácter
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
    else if (ft_strstr(ecmd->argv[0], "pwd"))
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
    else if (ft_strstr(ecmd->argv[0], "cd"))
    {
        // Implementación de cd
        // Validar comillas, expansión de variables y cambio de directorio
        // Asegúrate de manejar las validaciones y las variables de entorno correctamente.
    }
    else if (ft_strstr(ecmd->argv[0], "env"))
    {
        int i = 0;
        while (minishell->env[i])
        {
            write(1, minishell->env[i], ft_strlen(minishell->env[i]));
            write(1, "\n", 1);
            i++;
        }
    }
    else if (ft_strstr(ecmd->argv[0], "exit"))
    {
        exit(minishell->status);
    }
    else if (ft_strstr(ecmd->argv[0], "export"))
    {
        // Implementación de export (si es necesario)
    }
    else if (ft_strstr(ecmd->argv[0], "unset"))
    {
        // Implementación de unset (si es necesario)
    }
}

void run_external(t_cmd *cmd, t_minishell *minishell)
{
    int status;
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        runcmd(cmd, minishell);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        minishell->status = WEXITSTATUS(status);
    }
}

void print_vector(char **vector) {
    int i = 0;
    while (vector[i]) {
        printf("vector[%d]: %s\n", i, vector[i]);
        i++;
    }
}

int is_valid_quote(t_cmd *cmd, t_minishell *minishell)
{
	char quote;
	int idx;
	t_execcmd *ecmd;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	idx = 1;
	quote = 0;
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
		minishell->status = 1;
		return (0);
	}
	return (1);
}
