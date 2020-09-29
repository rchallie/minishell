/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_int_and_ptr.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 11:53:36 by thervieu          #+#    #+#             */
/*   Updated: 2019/11/27 11:54:13 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

size_t	manage_int(int i, t_flags flags)
{
	char	*nbr;
	int		save;
	size_t	count;

	save = i;
	count = 0;
	if (flags.dot == 0 && i == 0)
	{
		count += width(flags.width, 0, 0);
		return (count);
	}
	if (i < 0 && (flags.dot >= 0 || flags.zero == 1))
	{
		if (flags.zero == 1 && flags.dot == -1)
			put_str("-", 1);
		i *= -1;
		flags.zero = 1;
		flags.width--;
		count++;
	}
	nbr = ft_itoa(i);
	count += flags_put_str_for_diux(save, nbr, flags);
	free(nbr);
	return (count);
}

size_t	manage_pointer(unsigned long long u, t_flags flags)
{
	char	*ptr;
	size_t	count;

	count = 0;
	if (u == 0 && flags.dot == 0)
	{
		count += put_str("0x", 2);
		return (count += width(flags.width, 0, 1));
	}
	ptr = ull_itoa_base1(u, 16, 0);
	if (flags.dot < (int)ft_strlen(ptr))
		flags.dot = (int)ft_strlen(ptr);
	if (flags.minus == 1)
		count += flags_put_str_for_sp(ptr, flags);
	count += width(flags.width, ft_strlen(ptr) + 2, 0);
	if (flags.minus == 0)
		count += flags_put_str_for_sp(ptr, flags);
	free(ptr);
	return (count);
}
