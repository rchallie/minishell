/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 09:46:55 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/29 09:46:58 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

size_t	parse_conv(char c, va_list va, t_flags flags)
{
	if (c == '%')
		return (manage_percent(flags));
	if (is_conv_nbr((char)flags.conv))
	{
		if (c == 'd' || c == 'i'
			|| c == 'u')
			flags.base = 10;
		else if (c == 'x' || c == 'X'
			|| c == 'p')
			flags.base = 16;
		if (c == 'u' || c == 'x'
			|| c == 'X')
			return (manage_uint_and_hexa(va_arg(va, unsigned int), flags));
		else if (c == 'p')
			return (manage_pointer(va_arg(va, unsigned long long), flags));
		return (manage_int(va_arg(va, int), flags));
	}
	else if (c == 'c')
		return (manage_char((char)va_arg(va, int), flags));
	else if (c == 's')
		return (manage_string(va_arg(va, char *), flags));
	return (0);
}

size_t	parse_flags(const char *str, size_t i, va_list va, t_flags *flags)
{
	while (str[i] && (is_in_flags_or_conv(str[i]) == 1 || ft_isdigit(str[i])))
	{
		if (str[i] == '0' && flags->minus == 0 && flags->width == 0)
			flags->zero = 1;
		if (str[i] == '-')
			*flags = minus(*flags);
		if (str[i] == '.')
			i = dot(str, i, va, flags);
		if (str[i] == '*')
			*flags = ast(va, *flags);
		if (ft_isdigit(str[i]))
			*flags = digit(*flags, str[i]);
		if (is_conv(str[i]))
		{
			flags->conv = str[i];
			break ;
		}
		i++;
	}
	return (i);
}
