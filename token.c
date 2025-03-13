/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/13 16:26:58 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int gettoken(char **ps, char *es, char **q, char **eq) {
    char *s;
    int ret;

    s = *ps;
    while (s < es && strchr(whitespace, *s)) s++;

    if (q) *q = s;

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
            if (s < es && *s == '<') {  // Si encontramos otro '<', es un heredoc.
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
        default:
            ret = 'a';  // Identificador de palabra
            while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
                s++;
            break;
    }

    if (eq) *eq = s;

    while (s < es && strchr(whitespace, *s)) s++;
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
