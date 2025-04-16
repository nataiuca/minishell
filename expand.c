/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:58:39 by natferna          #+#    #+#             */
/*   Updated: 2025/03/27 16:59:00 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *extract_single_quotes(char **ps, char *es) {
    char quote = **ps;
    (*ps)++;
    char *start = *ps;
    while (*ps < es && **ps != quote)
        (*ps)++;
    if (*ps >= es || **ps != quote)
        error_exit("Error: Comilla de cierre no encontrada en comillas simples.\n");
    int len = *ps - start;
    char *token = malloc(len + 1);
    if (!token)
        error_exit("Error: malloc failed in extract_single_quotes.\n");
    memcpy(token, start, len);
    token[len] = '\0';
    (*ps)++;
    return token;
}

char *extract_double_quotes(char **ps, char *es) {
    char quote = **ps;
    (*ps)++;
    char *start = *ps;
    while (*ps < es && **ps != quote) {
        if (**ps == '\\' && *ps + 1 < es)
            (*ps)++;
        (*ps)++;
    }
    if (*ps >= es || **ps != quote)
        error_exit("Error: Comilla de cierre no encontrada en comillas dobles.\n");
    int len = *ps - start;
    char *token = malloc(len + 1);
    if (!token)
        error_exit("Error: malloc failed in extract_double_quotes.\n");
    memcpy(token, start, len);
    token[len] = '\0';
    (*ps)++;
    char *expanded = expand_variable(token);
    free(token);
    return expanded;
}

char *extract_token(char **ps, char *es) {
    char *token = NULL;
    if (**ps == '\'' || **ps == '"') {
        char current_quote = **ps;
        if (current_quote == '\'')
            token = extract_single_quotes(ps, es);
        else
            token = extract_double_quotes(ps, es);
    } else {
        const char *ws = " \t\r\n\v";
        const char *sy = "|&;()<>";
        char *start = *ps;
        while (*ps < es && !strchr(ws, **ps) && !strchr(sy, **ps))
            (*ps)++;
        int len = *ps - start;
        token = malloc(len + 1);
        if (!token)
            error_exit("Error: malloc failed in extract_token (no quotes).\n");
        memcpy(token, start, len);
        token[len] = '\0';
    }
    return token;
}
