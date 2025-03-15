/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 01:17:45 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_vector(char **vector)
{
	int	i;

	i = 0;
	while (vector[i])
	{
		printf("vector[%d]: %s\n", i, vector[i]);
		i++;
	}
}

int main(int argc, char **argv, char **envp) {
    t_minishell minishell;
    char *input;

    valid_inital_param(argc, envp, &minishell);
    catch_signal();

	/* Crea la estructura de historial y la carga desde disco */
    minishell.history = history_create();
    load_history_file(minishell.history, ".minishell_history");
    while (1) {
		if (isatty(fileno(stdin)))
			input = readline(PROMPT);
		else {
			char *line = get_next_line(fileno(stdin));
			if (!line)
				break;
			input = ft_strtrim(line, "\n");
			free(line);
		}
	
		if (!input)  // Ctrl+D o EOF
		{
			printf("exit\n");
			save_history_file(minishell.history, ".minishell_history", 1000);
			history_free(minishell.history);
			break;
		}
	
		if (input[0] != '\0') {
			add_history(input);
			history_add(minishell.history, input);
		}
	
		catch_interactive(minishell.history, input, PROMPT);
		input = check_input_valid(input);
		if (input[0] == '\0') {
			free(input);
			continue;
		}
	
		if (fork() == 0) {
			runcmd(parsecmd(input));
			exit(0);
		}
		wait(0);
		free(input);
	}
	
    safe_free_minishell(&minishell);
    return 0;
}
