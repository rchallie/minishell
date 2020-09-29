/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_percent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 12:54:22 by thervieu          #+#    #+#             */
/*   Updated: 2019/11/29 12:54:29 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

size_t	manage_percent(t_flags flags)
{
	size_t	count;

	count = 0;
	if (flags.minus == 1)
		count += put_str("%", 1);
	count += width(flags.width, 1, flags.zero);
	if (flags.minus == 0)
		count += put_str("%", 1);
	return (count);
}
