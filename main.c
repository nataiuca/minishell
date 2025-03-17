/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/17 13:49:45 by natferna         ###   ########.fr       */
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

    valid_inital_param(argc, argv, &minishell);
    catch_signal();

    // Crea la estructura de historial y la carga desde disco
    minishell.history = history_create();
    load_history_file(minishell.history, ".minishell_history");
    
    while (1) {
        input = readline("minishell$ ");
        if (!input) {
            printf("exit\n");
            save_history_file(minishell.history, ".minishell_history", 1000);
            history_free(minishell.history);
            break;
        }
        
        // Si la entrada no es vacía, la agregamos al historial siempre y cuando
        // no se trate de un comando de historial, para evitar contaminarlo.
        if (input[0] != '\0') {
            if (strncmp(input, "history", 7) != 0) {
                add_history(input);
                history_add(minishell.history, input);
            }
        }
        
        // Si el comando ingresado es "history" o similar, lo procesamos de forma especial:
        if (strncmp(input, "history", 7) == 0) {
            // Se extrae el argumento (si existe) después de "history"
            char *arg = input + 7;
            // Saltamos cualquier espacio en blanco
            while (*arg && ft_isspace((unsigned char)*arg))
                arg++;

            // Si el argumento no es vacío se lo pasa a history_print,
            // de lo contrario se imprime el historial completo.
            history_print(minishell.history, ((*arg) != '\0') ? arg : NULL);
            free(input);
            continue;
        }
        
        // Procesar otros comandos (en este ejemplo se usan funciones de tu minishell)
        catch_interactive(minishell.history, input, "minishell$ ");
        input = check_input_valid(input);
        
        if (fork() == 0) {
            runcmd(parsecmd(input));
            exit(0);
        }
        wait(NULL);
        free(input);
    }
    
    safe_free_minishell(&minishell);
    return 0;
}