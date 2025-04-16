/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:13:38 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:59:59 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(const char *input) {
    size_t buf_size = 128;
    char *result = malloc(buf_size);

    if (!result)
        error_exit("Error: malloc failed in expand_variables.\n");
    size_t res_index = 0, i = 0;
    while (input[i] != '\0') {
        if (input[i] == '$') {
            size_t j = i + 1;
            while (input[j] != '\0' && (ft_isalnum(input[j]) || input[j] == '_'))
                j++;
            int var_len = j - (i + 1);
            char var_name[var_len + 1];
            size_t k = 0;
            while (k < (size_t)var_len) {
                var_name[k] = input[i + 1 + k];
                k++;
            }
            var_name[var_len] = '\0';
            char *var_val = getenv(var_name);
            if (var_val) {
                size_t k2 = 0;
                while (var_val[k2] != '\0') {
                    if (res_index + 1 >= buf_size) {
                        buf_size *= 2;
                        result = realloc(result, buf_size);
                        if (!result)
                            error_exit("Error: realloc failed in expand_variables.\n");
                    }
                    result[res_index++] = var_val[k2];
                    k2++;
                }
            }
            i = j;
            continue;
        } else {
            if (res_index + 1 >= buf_size) {
                buf_size *= 2;
                result = realloc(result, buf_size);
                if (!result)
                    error_exit("Error: realloc failed in expand_variables.\n");
            }
            result[res_index++] = input[i];
            i++;
        }
    }
    result[res_index] = '\0';
    return result;
}
