/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:47:29 by natferna          #+#    #+#             */
/*   Updated: 2025/03/17 14:00:12 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void history_clear(t_history *hist) {
    int i = 0;
    if (!hist)
        return;
    while (i < hist->count) {
        free(hist->entries[i]);  // Cada hist->entries[i] debe ser un puntero asignado dinámicamente
        i++;
    }
    free(hist->entries);
    // Si decides re-inicializar:
    hist->entries = malloc(sizeof(char *) * hist->capacity);
    if (!hist->entries)
        return;
    hist->count = 0;
    clear_history();
}


void history_print(t_history *hist, const char *option) {
    int i = 0, start = 0;

    // Si el historial está vacío...
    if (!hist || hist->count == 0) {
        printf("Historial vacío.\n");
        return;
    }

    // Si no se pasó ninguna opción, se muestra todo el historial.
    if (option == NULL) {
        start = 0;
    }
    // Si se pasa la opción "-c", se limpia el historial.
    else if (strcmp(option, "-c") == 0) {
        history_clear(hist);
        printf("Historial borrado.\n");
        return;
    }
    // Si se pasa una opción en formato -X (donde X es un número).
    else if (option[0] == '-' && ft_isdigit((unsigned char)option[1])) {
        int x = atoi(option + 1);
        if (x > hist->count)
            x = hist->count;
        start = hist->count - x;
    }
    else {
        printf("Uso: history [-X] | history -c\n");
        return;
    }

    // Imprimir las líneas desde 'start' hasta el final usando while.
    i = start;
    while (i < hist->count) {
        printf("%d  %s\n", i + 1, hist->entries[i]);
        i++;
    }
}
