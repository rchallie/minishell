/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 20:58:51 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 13:00:57 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		get_double_char_tab_len(char **tabl)
{
	int i = 0;
	
	if (!tabl || !*tabl)
		return (0);
	while (tabl[i])
		i++;
	return (i);
}

int	ft_secure_strlen(const char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

int	ft_is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r');
}