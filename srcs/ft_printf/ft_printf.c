/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 14:00:20 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/29 09:49:40 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

t_flags	init_flags(t_flags flags)
{
	flags.minus = 0;
	flags.zero = 0;
	flags.dot = -1;
	flags.ast = 0;
	flags.width = 0;
	flags.base = 0;
	flags.count = 0;
	flags.conv = '0';
	return (flags);
}

size_t	write_c(char c)
{
	write(1, &c, 1);
	return (1);
}

size_t	parse_string(const char *str, va_list va)
{
	t_flags	flags;
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			flags = init_flags(flags);
			i = parse_flags(str, ++i, va, &flags);
			if (is_conv(str[i]))
				count += parse_conv(flags.conv, va, flags);
			else
				count += write_c(str[i]);
		}
		else
			count += write_c(str[i]);
		i++;
	}
	return (count);
}

int		ft_printf(const char *str, ...)
{
	va_list	va;

	va_start(va, str);
	if (!str)
		return (0);
	return (parse_string(str, va));
}
