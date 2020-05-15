/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_group_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 12:54:05 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/15 15:49:13 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

size_t	put_str(char *str, size_t size)
{
	size_t	count;

	count = 0;
	while (str[count] && count < size)
	{
		ft_putchar_fd(str[count], g_fd);
		count++;
	}
	return (count);
}

size_t	prior_put_str(int nb, char *str, t_flags flags)
{
	size_t	count;

	count = 0;
	if (nb < 0 && flags.dot >= 0 && (flags.conv == 'd' || flags.conv == 'i'))
		ft_putchar_fd('-', g_fd);
	if (flags.dot >= 0)
		count += width(flags.dot - 1, ft_strlen(str) - 1, 1);
	count += put_str(str, ft_strlen(str));
	return (count);
}

size_t	flags_put_str_for_diux(int save, char *str, t_flags flags)
{
	size_t	count;

	count = 0;
	if (flags.minus == 1)
		count += prior_put_str(save, str, flags);
	if (flags.dot >= 0 && flags.dot < (int)ft_strlen(str))
		flags.dot = (int)ft_strlen(str);
	if (flags.dot >= 0)
	{
		flags.width -= flags.dot;
		count += width(flags.width, 0, 0);
	}
	else
	{
		count += width(flags.width, ft_strlen(str), flags.zero);
	}
	if (flags.minus == 0)
		count += prior_put_str(save, str, flags);
	return (count);
}

size_t	flags_put_str_for_sp(char *str, t_flags flags)
{
	size_t	count;

	count = 0;
	if (flags.conv == 'p')
		count += put_str("0x", 2);
	if (flags.dot >= 0 && flags.conv == 'p')
	{
		count += width(flags.dot, ft_strlen(str), 1);
		count += put_str(str, flags.dot);
	}
	else if (flags.dot >= 0 && flags.conv == 's')
	{
		count += width(flags.dot, ft_strlen(str), 0);
		count += put_str(str, flags.dot);
	}
	else
		count += put_str(str, ft_strlen(str));
	return (count);
}
