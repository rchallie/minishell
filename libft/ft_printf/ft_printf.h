/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 09:53:22 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/29 22:07:55 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include "../libft.h"

typedef struct	s_flags
{
	size_t	minus;
	size_t	zero;
	int		dot;
	size_t	ast;
	int		width;
	size_t	base;
	size_t	count;
	char	conv;
}				t_flags;

int				g_fd;

int				ft_printf(int fd, const char *str, ...);
size_t			is_conv(char c);
size_t			is_conv_nbr(char c);
size_t			is_in_flags_or_conv(char c);
size_t			parse_flags(const char *str, size_t i, va_list va,
	t_flags *flags);
size_t			parse_conv(char c, va_list va, t_flags flags);
t_flags			minus(t_flags flags);
t_flags			digit(t_flags flags, char c);
t_flags			ast(va_list va, t_flags flags);
size_t			width(int width, int minus, size_t zero);
size_t			dot(const char *str, size_t i, va_list va, t_flags *flags);
size_t			manage_char(char c, t_flags flags);
size_t			manage_string(char *str, t_flags flags);
size_t			manage_int(int i, t_flags flags);
size_t			manage_pointer(unsigned long long u, t_flags flags);
size_t			manage_uint_and_hexa(unsigned int unint, t_flags flags);
char			*ull_itoa_base1(unsigned long long ull, size_t base,
	size_t upper);
size_t			manage_percent(t_flags flags);
size_t			put_str(char *str, size_t size);
size_t			prior_put_str(int save, char *str, t_flags flags);
size_t			flags_put_str_for_diux(int save, char *str, t_flags flags);
size_t			flags_put_str_for_sp(char *str, t_flags flags);

#endif
