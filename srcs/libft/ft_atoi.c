/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 12:08:55 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/23 16:20:11 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\v'
			|| c == '\n' || c == '\f')
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int		i;
	int		nb;
	int		sign;

	i = 0;
	if (str == NULL && *str == '0')
		return (0);
	sign = 0;
	nb = 0;
	while (is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign++;
		i++;
	}
	while (ft_isdigit(str[i]))
		nb = (nb * 10) + (str[i++] - '0');
	if (sign == 1)
		return (-nb);
	else
		return (nb);
}
