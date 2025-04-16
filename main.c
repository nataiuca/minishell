/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:08:51 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:54:29 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp) {
    t_minishell minishell;
    char *input;
    valid_inital_param(argc, envp, &minishell);
    catch_signal();
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
        if (input[0] != '\0') {
            if (strncmp(input, "history", 7) != 0) {
                add_history(input);
                history_add(minishell.history, input);
            }
        }
        if (strncmp(input, "history", 7) == 0) {
            char *arg = input + 7;
            while (*arg && ft_isspace((unsigned char)*arg))
                arg++;
            history_print(minishell.history, ((*arg) != '\0') ? arg : NULL);
            free(input);
            continue;
        }
        catch_interactive(minishell.history, input, "minishell$ ");
        input = check_input_valid(input);
        if (fork() == 0) {
            runcmd(parsecmd(input), &minishell);
            exit(0);
        }
        wait(NULL);
        free(input);
    }
    safe_free_minishell(&minishell);
    return 0;
}
