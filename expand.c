/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:35:18 by natferna          #+#    #+#             */
/*   Updated: 2025/03/24 18:27:47 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función para expandir variables SIN usar for (utilizando while)
char *expand_variables(const char *input) {
    size_t buf_size = 128;
    char *result = malloc(buf_size);
    if (!result)
        error_exit("Error: malloc failed in expand_variables.\n");
    size_t res_index = 0;
    
    size_t i = 0;
    while (input[i] != '\0') {
        if (input[i] == '$') {
            size_t j = i + 1;
            // Recolecta caracteres válidos para el nombre de la variable.
            while (input[j] != '\0' && (isalnum(input[j]) || input[j] == '_'))
                j++;
            int var_len = j - (i + 1);
            
            // Copia el nombre de la variable usando while en lugar de for
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
            i = j;  // Avanza el índice más allá del nombre de la variable
            continue;  // Continua al siguiente carácter
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

// -----------------------------------------------------------------
// Extracción de tokens entre comillas simples
char *extract_single_quotes(char **ps, char *es) {
    char quote = **ps;  // Debe ser '\''
    (*ps)++;            // Salta la comilla de apertura
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
    (*ps)++;  // Salta la comilla de cierre
    
    // En comillas simples no se realiza expansión
    return token;
}

// -----------------------------------------------------------------
// Extracción de tokens entre comillas dobles
char *extract_double_quotes(char **ps, char *es) {
    char quote = **ps;  // Debe ser '"'
    (*ps)++;            // Salta la comilla de apertura
    char *start = *ps;
    
    while (*ps < es && **ps != quote) {
        // Manejo básico de secuencias de escape: si se encuentra '\' se salta ese caracter
        if (**ps == '\\' && *ps + 1 < es) {
            (*ps)++;  // Salta el '\' y copia el siguiente carácter literalmente
        }
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
    (*ps)++;  // Salta la comilla de cierre
    
    // Expande variables dentro del token
    char *expanded = expand_variables(token);
    free(token);
    return expanded;
}

// -----------------------------------------------------------------
// Función principal para extraer el siguiente token de la entrada
char *extract_token(char **ps, char *es) {
    char *token = NULL;
    
    if (**ps == '\'' || **ps == '"') {
        char current_quote = **ps;
        if (current_quote == '\'')
            token = extract_single_quotes(ps, es);
        else // current_quote == '"'
            token = extract_double_quotes(ps, es);
    } else {
        // Procesa token sin comillas: copia hasta llegar a un espacio o símbolo
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