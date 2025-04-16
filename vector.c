/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:13:08 by natferna          #+#    #+#             */
/*   Updated: 2025/03/26 14:13:10 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_vector_add_first(char **vector, char *new) {
    int i = 0;
    while (vector[i])
        i++;
    char **new_vector = (char **)safe_malloc(sizeof(char *) * (i + 2));
    new_vector[0] = new;
    i = 0;
    while (vector[i]) {
        new_vector[i + 1] = vector[i];
        i++;
    }
    new_vector[i + 1] = NULL;
    return new_vector;
}

void ft_vector_remove_last_element(char **argv) {
    int i = 0;
    while (argv[i])
        i++;
    free(argv[i - 1]);
    argv[i - 1] = NULL;
}

void ft_vector_trim(char **argv) {
    int i = 0;
    char *tmp;
    while (argv[i]) {
        tmp = ft_strtrim(argv[i], " ");
        free(argv[i]);
        argv[i] = tmp;
        i++;
    }
}

int ft_vector_size(char **split) {
    int i = 0;
    while (split[i])
        i++;
    return i;
}

void replace_element_index(char **split, int index, char *tmp) {
    free(split[index]);
    split[index] = tmp;
}
