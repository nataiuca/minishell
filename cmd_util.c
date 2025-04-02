/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 22:35:51 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/01 00:32:15 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_variable(t_cmd *cmd, int idx, int *pos, t_minishell *minishell) {
    // Verificar que cmd y pos sean válidos
    if (!cmd || !pos)
        return;

    t_execcmd *ecmd = (t_execcmd *)cmd;
    if (!ecmd || idx < 0 || !ecmd->argv[idx])
        return;

    char *token = ecmd->argv[idx];
    // Verificar que pos esté dentro del rango y que apunte a un '$'
    if (!token || *pos < 0 || *pos >= (int)strlen(token) || token[*pos] != '$')
        return;

    int var_start = *pos + 1;
    int var_len = 0;

    // Calcular la longitud del nombre de la variable
    while (token[var_start + var_len] &&
           (isalnum(token[var_start + var_len]) || token[var_start + var_len] == '_')) {
        var_len++;
    }

    if (var_len == 0)  // Si no se encontró una variable válida
        return;

    char *var_name = ft_substr(token, var_start, var_len);
    if (!var_name) {
        perror("malloc failed for var_name");
        exit(EXIT_FAILURE);
    }

    char *value = getenv_minishell(minishell, var_name);
    free(var_name);
    if (!value)
        value = "";  // Usar cadena vacía si la variable no existe

    int prefix_len = *pos;  // Parte del token anterior al '$'
    char *prefix = ft_substr(token, 0, prefix_len);
    if (!prefix) {
        perror("malloc failed for prefix");
        exit(EXIT_FAILURE);
    }

    char *suffix = ft_substr(token, var_start + var_len, strlen(token) - (var_start + var_len));
    if (!suffix) {
        free(prefix);
        perror("malloc failed for suffix");
        exit(EXIT_FAILURE);
    }

    char *temp = ft_strjoin(prefix, value);
    if (!temp) {
        free(prefix);
        free(suffix);
        perror("malloc failed for temp");
        exit(EXIT_FAILURE);
    }

    char *new_token = ft_strjoin(temp, suffix);
    if (!new_token) {
        free(prefix);
        free(suffix);
        free(temp);
        perror("malloc failed for new_token");
        exit(EXIT_FAILURE);
    }

    free(prefix);
    free(suffix);
    free(temp);
    free(ecmd->argv[idx]);

    ecmd->argv[idx] = new_token;
    *pos = prefix_len + strlen(value);  // Actualizar pos al final del valor expandido
}


char *expand_token(const char *token, t_minishell *minishell) {
    char *result = NULL;
    size_t result_size = 0;
    
    // Inicializamos resultado vacío.
    result = malloc(1);
    if (!result)
        panic("malloc failed");
    result[0] = '\0';
    result_size = 1;

    while (*token) {
        if (*token == '$') {
            token++;  // saltar el '$'
            // Extraer nombre de la variable.
            size_t var_len = 0;
            while (token[var_len] && (ft_isalnum(token[var_len]) || token[var_len] == '_'))
                var_len++;

            char *var_name = ft_substr(token, 0, var_len);
            char *value = getenv_minishell(minishell, var_name);
            free(var_name);
            if (!value)
                value = ""; // Si la variable no existe, se toma como valor vacío.
            
            size_t value_len = ft_strlen(value);
            // Reallocar result para acomodar el nuevo valor.
            result = realloc(result, result_size + value_len);
            if (!result)
                panic("realloc failed");
            strcat(result, value);
            result_size += value_len;
            
            token += var_len;
        } else {
            // Copiar el carácter actual.
            char temp[2] = {*token, '\0'};
            result = realloc(result, result_size + 1);
            if (!result)
                panic("realloc failed");
            strcat(result, temp);
            result_size += 1;
            token++;
        }
    }
    return result;
}

int is_valid_quote(t_cmd *cmd, t_minishell *minishell)
{
	char quote;
	int idx;
	t_execcmd *ecmd;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	idx = 1;
	quote = 0;
	while (ecmd->argv[idx])
	{
		pos = 0;
		while (ecmd->argv[idx][pos])
		{
			if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
			{
				quote = ecmd->argv[idx][pos]; 
			}
			else if (quote && ecmd->argv[idx][pos] == quote)
			{
				quote = 0;
			}
			else if (quote == '"' && ecmd->argv[idx][pos] == '\\' && ecmd->argv[idx][pos + 1] == '"')
			{
				pos++;  // Saltar `\` pero mantener `"`
			}
		}
		idx++;
	}
	if (quote)
	{
		write(2, "Error: Unmatched quote\n", 23);
		minishell->status = 1;
		return (0);
	}
	return (1);
}

char *getenv_minishell(t_minishell *minishell, char *key) {
    if (!minishell || !minishell->env || !key) {
        return NULL; // Retorna NULL si algún puntero es inválido
    }

    int len = ft_strlen(key);
    for (int i = 0; minishell->env[i]; i++) {
        if (ft_strncmp(minishell->env[i], key, len) == 0 && minishell->env[i][len] == '=') {
            return &minishell->env[i][len + 1];
        }
    }

    return NULL; // Retorna NULL si la variable no existe
}
