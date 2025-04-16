/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:09:44 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 15:22:23 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

extern void rl_replace_line(const char *text, int clear_undo);

static void sigint_handler(int signal) {
    (void)signal;
    rl_replace_line("", 0);
    rl_on_new_line();
    printf("\n");
    rl_redisplay();
}

void catch_signal(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

void catch_interactive(t_history *hist, char *input, char *prompt) {
    if (!input)
        input = ft_strdup("exit");
    if (strcmp(input, "exit") == 0) {
        save_history_file(hist, ".minishell_history", 1000);
        history_free(hist);
        free(input);
        printf("\033[1A");
        printf("\033[%zuC", ft_strlen(prompt));
        printf("exit\n");
        exit(0);
    }
}

void save_history(char *input) {
    if (input[0] != '\0')
        add_history(input);
}

char *check_input_valid(char *input) {
    if (input[0] != '\0') {
        input = trim_space_char(input);
        save_history(input);
    }
    return input;
}
