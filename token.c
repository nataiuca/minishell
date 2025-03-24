/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/24 22:45:02 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

void error_exit(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    exit(EXIT_FAILURE);
}

int gettoken(char **ps, char *es, char **q, char **eq) {
    char *s = *ps;
    int ret;

    // Saltar espacios en blanco
    while (s < es && strchr(whitespace, *s))
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
            while (s < es && !strchr(whitespace, *s) && !strchr("|&;()<>", *s))
                s++;
            break;
    }

    if (eq)
        *eq = s;

    // Saltar espacios en blanco para el siguiente token
    while (s < es && strchr(whitespace, *s))
        s++;
    *ps = s;

    return ret;
}

int	peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && ft_strchr(" \t\r\n\v", *s))
    s++;
  *ps = s;
  return *s && ft_strchr(toks, *s);
}
