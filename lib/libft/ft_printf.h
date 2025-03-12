/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:02:12 by jgamarra          #+#    #+#             */
/*   Updated: 2024/09/03 00:24:44 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>
# include <unistd.h>

int	valid_length(int *size, int width, char c);
int	ft_putchar_size(char c, int fd);
int	ft_putstr_size(char *s, int fd);
int	ft_putnbr_size(int n, int fd);
int	ft_putunsignbr_fd(unsigned int n, int fd);
int	print_rep_char(char c, int fill);
int	get_length(char **str);
int	print_str(int width, char *val, char c);
int	print_chr(int width, int val, char c);
int	print_porcentage(int width, char c);
int	print_numeric(int width, int val, char c);
int	print_usignedint(int width, unsigned int val, char c);
int	print_hexadecimal(char str, int width, unsigned long long val, char c);
int	convert_hexadecimal(unsigned long long val, char c);
int	print_pointer_dimension(int width, unsigned long long pointer, char c);
int	ft_printf(char const *s, ...);

#endif