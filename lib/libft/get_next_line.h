/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 23:01:23 by jgamarra          #+#    #+#             */
/*   Updated: 2024/08/31 18:34:53 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <fcntl.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>

void	*ft_calloc_zero(size_t count, size_t size);
size_t	ft_strlen_gnl(const char *s);
size_t	ft_strappend(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin_free(char *s1, char const *s2);
char	*gnl_substr(char const *s, unsigned int start, size_t len);
char	*split_line_by_break(char **remainder_line);
char	*get_next_line(int fd);
void	*return_free_var(char **var);
char	*get_buffer_line(int fd, char *remainder_line);
int		ft_strchr_index(const char *s, int c);

#endif