/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/13 00:11:45 by natferna         ###   ########.fr       */
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
    while (1) {
        // REPLACE COMMENT FOR TESTER
        if (isatty(fileno(stdin)))
            input = readline(PROMPT);
        else {
            char *line;
            line = get_next_line(fileno(stdin));
            if (!line)
                break;
            input = ft_strtrim(line, "\n");
            free(line);
        }
        // REPLACE COMMENT FOR TESTER
        catch_interactive(input, PROMPT);
        input = check_input_valid(input);
        if (input[0] == '\0') {
            free(input);
            continue;
        }
        // pgm opt1 opt2 < file1 > file2
        // check if built-ins must be run by the child or the parent
        if (fork() == 0) {
            runcmd(parsecmd(input));
            exit(0); // Evitar que el hijo entre en el bucle
        }
        wait(0); //custom
        free(input);
    }
    safe_free_minishell(&minishell);
    return 0;
}
