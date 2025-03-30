/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 22:35:51 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/30 23:59:57 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_variable(t_cmd *cmd, int idx, int *pos, t_minishell *minishell)
{
    t_execcmd *ecmd = (t_execcmd *)cmd;
    char *token = ecmd->argv[idx];

    // Asegurarse de que token sea válido y que *pos sea una posición dentro de él.
    if (!token || !token[*pos])
        return;

    // Verificar que en la posición *pos se encuentre el símbolo '$'
    if (token[*pos] != '$')
        return;

    int var_start = *pos + 1;
    int var_len = 0;
    while (token[var_start + var_len] &&
           (ft_isalnum(token[var_start + var_len]) || token[var_start + var_len] == '_'))
    {
        var_len++;
    }

    // Extrae el nombre de la variable (sin el '$').
    char *var_name = ft_substr(token, var_start, var_len);
    // Obtén el valor de la variable usando tu función (si no existe, se usará cadena vacía).
    char *value = getenv_minishell(minishell, var_name);
    free(var_name);
    if (!value)
        value = "";  

    // Separa el token en tres partes: prefix (antes del '$'), value y suffix (después de la variable)
    int prefix_len = *pos;  // la parte anterior al '$'
    char *prefix = ft_substr(token, 0, prefix_len);
    char *suffix = ft_substr(token, var_start + var_len, ft_strlen(token) - (var_start + var_len));

    // Concatena: prefix + value + suffix
    char *temp = ft_strjoin(prefix, value);
    char *new_token = ft_strjoin(temp, suffix);

    free(prefix);
    free(suffix);
    free(temp);
    free(ecmd->argv[idx]); // Libera el token original
    ecmd->argv[idx] = new_token; // Guarda el token expandido

    // Actualiza la posición: se mueve al final de la parte insertada (esto es útil si hay más variables en el token)
    *pos = prefix_len + ft_strlen(value);
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
				quote = ecmd->argv[idx][pos];
			else if (quote && ecmd->argv[idx][pos] == quote)
				quote = 0;
			pos++;
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

char *getenv_minishell(t_minishell *minishell, char *key)
{
	int len = ft_strlen(key);
	if (!minishell /* o !minishell->env, según cómo lo implementes */) {
        return NULL;
    }
	for (int i = 0; minishell->env[i]; i++)
	{
		if (ft_strncmp(minishell->env[i], key, len) == 0 && minishell->env[i][len] == '=')
			return (&minishell->env[i][len + 1]);
	}
	return (0);
}
