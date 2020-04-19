/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_uint_and_hexa.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 11:53:36 by thervieu          #+#    #+#             */
/*   Updated: 2020/04/18 15:19:14 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

char	*str_to_upper(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 32;
		i++;
	}
	return (str);
}

char	*ull_itoa_base2(unsigned long long ull, size_t base, char *nbr,
	size_t count)
{
	while (ull != 0)
	{
		if ((ull % base) < 10)
			nbr[count] = (ull % base) + 48;
		else
			nbr[count] = (ull % base) + 87;
		ull /= base;
		count--;
	}
	return (nbr);
}

char	*ull_itoa_base1(unsigned long long ull, size_t base, size_t upper)
{
	char				*nbr;
	unsigned long long	save;
	size_t				count;

	save = ull;
	count = 0;
	if (ull == 0)
		return (ft_strdup("0"));
	while (ull != 0)
	{
		ull /= base;
		count++;
	}
	nbr = 0;
	if (!(nbr = malloc(sizeof(char) * (count + 1))))
		return (0);
	nbr[count] = '\0';
	nbr = ull_itoa_base2(save, base, nbr, --count);
	if (upper == 1)
		str_to_upper(nbr);
	return (nbr);
}

size_t	manage_uint_and_hexa(unsigned int unint, t_flags flags)
{
	char	*nbr;
	size_t	count;

	count = 0;
	// if (unint < 0)
	// 	unint = 4294967295 + 1 + unint;
	if (flags.dot == 0 && unint == 0)
	{
		count += width(flags.width, 0, 0);
		return (count);
	}
	if (flags.conv == 'u')
		nbr = ull_itoa_base1(unint, 10, 0);
	else if (flags.conv == 'x')
		nbr = ull_itoa_base1((unsigned long long)unint, 16, 0);
	else
		nbr = ull_itoa_base1((unsigned long long)unint, 16, 1);
	count += flags_put_str_for_diux(unint, nbr, flags);
	free(nbr);
	return (count);
}
