/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/01 00:33:01 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>";

int gettoken(char **ps, char *es, char **q, char **eq) {
    char *s = *ps;
    int ret;

    // Saltar espacios en blanco
    while (s < es && ft_strchr(whitespace, *s))
        s++;

    if (q)
        *q = s;

    ret = *s;
    switch (*s) {
        case 0:
            break;
        case '|':
        case '&':
        case ';':
        case '(':
        case ')':
            s++;
            break;
        case '<':
            s++;
            if (s < es && *s == '<') {
                ret = HDOC;
                s++;
            }
            break;
        case '>':
            s++;
            if (s < es && *s == '>') {
                ret = '+';
                s++;
            }
            break;
        case '"':
        case '\'':
        {
            char quote = *s;
            if (q)
                *q = s + 1; // Omitimos la comilla inicial
            s++;

            while (s < es) {
                if (*s == '\\' && s + 1 < es) {
                    if (quote == '"' && (s[1] == '"' || s[1] == '\\' || s[1] == '$' || s[1] == '`')) {
                        s++;  // Mantenemos el carácter escapado, saltando el `\`
                    }
                } else if (*s == quote) {
                    break;  // Encontramos la comilla de cierre
                }
                s++;
            }
            if (s >= es || *s != quote)
                error_exit("Error: Comilla de cierre no encontrada.\n");

            ret = 'a';
            s++;  // Saltar la comilla de cierre
        }
        break;
        default:
            ret = 'a';
            while (s < es && !ft_strchr(whitespace, *s) && !ft_strchr("|&;()<>", *s))
                s++;
            break;
    }

    if (eq)
        *eq = s;

    // Saltar espacios en blanco
    while (s < es && ft_strchr(whitespace, *s))
        s++;
    *ps = s;

    return ret;
}

int	peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && strchr(" \t\r\n\v", *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

char *strip_quotes(const char *token, int len) {
    char *result = malloc(len + 1);
    if (!result)
        error_exit("Error: malloc failed.\n");

    int i = 0, j = 0;
    int in_double = 0, in_single = 0;

    while (i < len) {
        char c = token[i];

        // Manejo de secuencias de escape dentro de comillas dobles
        if (c == '\\' && i + 1 < len) {
            char next = token[i + 1];

            if (in_double && (next == '"' || next == '\\' || next == '$' || next == '`')) {
                result[j++] = next;  // Copiamos el carácter escapado, omitiendo el `\`
                i += 2;
                continue;
            } else if (!in_single) {
                // Fuera de comillas simples, copiamos el `\` y el carácter escapado
                result[j++] = c;
                result[j++] = next;
                i += 2;
                continue;
            }
        }

        // Manejo de comillas dobles
        if (c == '"' && !in_single) {
            in_double = !in_double;  // Alternamos el estado de comillas dobles
            i++;
            continue;
        }
        // Manejo de comillas simples
        if (c == '\'' && !in_double) {
            in_single = !in_single;  // Alternamos el estado de comillas simples
            i++;
            continue;
        }

        // Copiamos cualquier otro carácter
        result[j++] = c;
        i++;
    }
    result[j] = '\0';
    return result;
}

int  tokenize_char(char c, char *quote)
{
    if (*quote == 0 && (c == '\'' || c == '"'))
    {
        *quote = c;
        return (0);
    }
    else if (*quote && c == *quote)
    {
        *quote = 0;
        return (0);
    }
    return (1); // Devuelve 1 para que se interprete normalmente
} 

void error_exit(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    exit(EXIT_FAILURE);
}