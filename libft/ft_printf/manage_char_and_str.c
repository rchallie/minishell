/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_char_and_str.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 14:36:28 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/15 15:48:17 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

size_t	manage_char(char c, t_flags flags)
{
	size_t	count;

	count = 0;
	if (flags.minus == 1)
		ft_putchar_fd(c, g_fd);
	count += width(flags.width, 1, 0);
	if (flags.minus == 0)
		ft_putchar_fd(c, g_fd);
	return (count + 1);
}

size_t	manage_string(char *str, t_flags flags)
{
	size_t	count;

	count = 0;
	if (!str)
		str = "(null)";
	if (flags.dot >= 0 && flags.dot > (int)ft_strlen(str))
		flags.dot = (int)ft_strlen(str);
	if (flags.minus == 1)
		count += flags_put_str_for_sp(str, flags);
	if (flags.dot >= 0)
		count += width(flags.width, flags.dot, 0);
	else
		count += width(flags.width, ft_strlen(str), 0);
	if (flags.minus == 0)
		count += flags_put_str_for_sp(str, flags);
	return (count);
}
