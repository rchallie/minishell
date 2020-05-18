/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 11:04:51 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/15 15:48:44 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

t_flags	minus(t_flags flags)
{
	flags.minus = 1;
	flags.zero = 0;
	return (flags);
}

t_flags	digit(t_flags flags, char c)
{
	if (flags.ast == 1)
		flags.width = 0;
	flags.width = (flags.width * 10) + (c - '0');
	return (flags);
}

t_flags	ast(va_list va, t_flags flags)
{
	flags.ast = 1;
	flags.width = va_arg(va, size_t);
	if (flags.width < 0)
	{
		flags.minus = 1;
		flags.width *= -1;
	}
	return (flags);
}

size_t	width(int width, int minus, size_t zero)
{
	size_t	count;

	count = 0;
	while (width - minus > 0)
	{
		if (zero)
			ft_putchar_fd('0', g_fd);
		else
			ft_putchar_fd(' ', g_fd);
		width--;
		count++;
	}
	return (count);
}

size_t	dot(const char *str, size_t i, va_list va, t_flags *flags)
{
	size_t	j;

	j = i;
	j++;
	if (str[j] == '*')
	{
		flags->dot = va_arg(va, size_t);
		if (flags->dot < 0)
			flags->dot *= -1;
		j++;
	}
	else
	{
		flags->dot = 0;
		while (ft_isdigit(str[j]))
		{
			flags->dot = (flags->dot * 10) + (str[j] - '0');
			j++;
		}
	}
	return (j);
}
