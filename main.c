/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 18:59:06 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_minishell minishell;
    char        *input;
    struct cmd  *cmd;

    (void)argv;
    prepare_minishell(&minishell);
    valid_inital_param(argc, envp, &minishell);

    minishell.history = history_create();
    if (!minishell.history)
        ft_exit_message("Error: No se pudo crear el historial.\n", 1);
    load_history_file(minishell.history, ".minishell_history");

    catch_signal();
    while (1)
    {
        if (isatty(fileno(stdin)))
        {
            input = readline(PROMPT);
            if (!input)  // Si readline retorna NULL
            {
                printf("exit\n");
                save_history_file(minishell.history, ".minishell_history", 1000);
                history_clear(minishell.history);
                break;
            }
        }
        else
        {
            char *line = get_next_line(fileno(stdin));
            if (!line)
            {
                printf("exit\n");
                save_history_file(minishell.history, ".minishell_history", 1000);
                history_clear(minishell.history);
                break;
            }
            input = ft_strtrim(line, "\n");
            free(line);
        }

        input = check_input_valid(input);
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        /* Evitar agregar comandos que comiencen con "history" */
        char *trimmed = ft_strtrim(input, " \t");
        if (ft_strncmp(trimmed, "history", 7) != 0 
            && minishell.history_disabled == 0)
            history_add(minishell.history, input);
        free(trimmed);

        cmd = parsecmd(input);
        control_cmd(cmd, &minishell);
        free(input);
    }

    safe_free_minishell(&minishell);
    return (0);
}
