/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:47:29 by natferna          #+#    #+#             */
/*   Updated: 2025/04/02 20:19:49 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void history_clear(t_history *hist) {
    int i;
    char *path = construct_history_path(".minishell_history");

    if (!hist)
        return;

    for (i = 0; i < hist->count; i++)
        free(hist->entries[i]);
    free(hist->entries);

    hist->entries = malloc(sizeof(char *) * hist->capacity);
    if (!hist->entries)
        ft_exit_message("Error: Malloc falló en history_clear.\n", 1);

    hist->count = 0;
    clear_history();  // Borra historial de readline()

    if (path) {
        int fd = open(path, O_WRONLY | O_TRUNC);
        if (fd >= 0)
            close(fd);
        free(path);
    }
}


void history_print(t_history *hist, const char *option) {
    int start, i, num_entries;

    if (!hist || hist->count == 0) {
        printf("Historial vacío.\n");
        return;
    }

    if (option && option[0] == '-' && ft_isdigit((unsigned char)option[1])) {
        num_entries = atoi(option + 1);
        if (num_entries <= 0 || num_entries > hist->count)
            num_entries = hist->count;
        start = hist->count - num_entries;
    } else {
        start = 0;
    }

    for (i = start; i < hist->count; i++) {
        printf("%d  %s\n", i + 1, hist->entries[i]);
    }
}


void history_free(t_history *hist)
{
    if (!hist)
        return;
    for (int i = 0; i < hist->count; i++)
        free(hist->entries[i]);
    free(hist->entries);
    free(hist);
}