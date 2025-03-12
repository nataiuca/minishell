/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 23:01:18 by jgamarra          #+#    #+#             */
/*   Updated: 2024/08/31 18:35:19 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr_index(const char *s, int c)
{
	int	i;

	if (!s || s[0] == '\0')
		return (-1);
	i = 0;
	while (*s != '\0')
	{
		if (*s == (char)c)
			return (i);
		s++;
		i++;
	}
	if (*s == (char)c)
		return (i);
	return (-1);
}

char	*get_buffer_line(int fd, char *remainder_line)
{
	int		status;
	char	*buffer_read;

	status = 1;
	buffer_read = ft_calloc_zero(sizeof(char), BUFFER_SIZE + 1);
	while ((status != 0) && (ft_strchr_index(remainder_line, '\n') == -1))
	{
		status = read(fd, buffer_read, BUFFER_SIZE);
		if (status == -1)
		{
			free(remainder_line);
			remainder_line = NULL;
			free(buffer_read);
			return (NULL);
		}
		buffer_read[status] = '\0';
		remainder_line = ft_strjoin_free(remainder_line, buffer_read);
	}
	free(buffer_read);
	return (remainder_line);
}

void	*return_free_var(char **var)
{
	free(*var);
	*var = NULL;
	return (NULL);
}

char	*split_line_by_break(char **remainder_line)
{
	char	*line;
	char	*tmp;

	if (*remainder_line[0] == '\0')
		return (return_free_var(&*remainder_line));
	if (ft_strchr_index(*remainder_line, '\n') == -1)
	{
		line = gnl_substr(*remainder_line, 0, ft_strlen_gnl(*remainder_line));
		return_free_var(&*remainder_line);
	}
	else
	{
		line = gnl_substr(*remainder_line, 0,
				ft_strchr_index(*remainder_line, '\n') + 1);
		if (!line)
			return (return_free_var(&*remainder_line));
		tmp = gnl_substr(*remainder_line, ft_strchr_index(*remainder_line, '\n')
				+ 1, ft_strlen_gnl(*remainder_line));
		if (!tmp)
			return (return_free_var(&*remainder_line));
		free(*remainder_line);
		*remainder_line = gnl_substr(tmp, 0, ft_strlen_gnl(tmp));
		free(tmp);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*remainder_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	remainder_line = get_buffer_line(fd, remainder_line);
	if (!remainder_line)
		return (NULL);
	line = split_line_by_break(&remainder_line);
	return (line);
}

// int	main(void)
// {
// 	// char	*file = "./get_next_line.h";
// 	char	u_stdin[256];

// 	int 	file_descriptor;
// 	// int 	file_descriptor = open(stdin, O_RDONLY);
// 	char 	*line;
//     int count = 1;

// 	scanf("%255s",u_stdin);
// 	file_descriptor = open(u_stdin, O_RDONLY);
// 	printf("file_descriptor{%i}\n", file_descriptor);
// 	printf("u_stdin{%s}\n", u_stdin);
//     // export MallocStackLogging=1
//     // leaks --atExit --list -- ./a.out
// 	while ((line = get_next_line(file_descriptor)) != NULL)
// 	{
// 		printf("{%i}", count);
//         printf("((%s))", line);
//         count++;
// 		free(line);
// 	}
// 	// line = get_next_line(file_descriptor);
// 	// printf("{%i}", count)
// 	// printf("(%s)", line);
// 	printf("[%s]", line);
// 	free(line);
// 	close(file_descriptor);
// 	return (0);
// }
