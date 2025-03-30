/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 21:44:06 by natferna         ###   ########.fr       */
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
				char quote = *s;  // Guarda la comilla (por ejemplo, '"')
				if (q)
					*q = s;       // Deja que q apunte al inicio (la comilla)
				s++;              // Salta la comilla de apertura
				while (s < es && *s != quote)
					s++;
				if (s >= es || *s != quote)
					error_exit("Error: Comilla de cierre no encontrada.\n");
				ret = 'a';
				s++;  // Salta la comilla de cierre
			}
			break;
			
        default:
            ret = 'a';  // Token de tipo palabra
            while (s < es && !ft_strchr(whitespace, *s) && !ft_strchr("|&;()<>", *s))
                s++;
            break;
    }

    if (eq)
        *eq = s;

    // Saltar espacios en blanco para el siguiente token
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

char *strip_quotes(char *token, int len) {
    char quote = 0;
    char *result = malloc(len + 1); // Reserva la memoria necesaria
    if (!result)
        error_exit("Error: malloc failed.\n");
    
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (tokenize_char(token[i], &quote)) { // Solo copia los caracteres que no son comillas
            result[j++] = token[i];
        }
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