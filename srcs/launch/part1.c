/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 19:28:20 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/27 19:28:33 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void    welcome_to_minishell(char *launching, t_line line, char *str, int nb)
{
    int	i;
	int j;
    int delay;

	delay = 0;
    line.start.row = line.winsz.row / 2 + nb;
	line.start.col = line.winsz.col / 2 - ft_secure_strlen(launching) / 2;
    delay = (nb == -1) ? 1000 : 700; 
	while (ft_secure_strlen(str) < ft_secure_strlen(launching))
	{
		i = 0;
		str = add_char_to_word(str, launching[ft_secure_strlen(str)]);
		set_curpos(&line);
		ft_putstr_fd(str, 0);
		while (i < 100000)
		{
			j = 0;
			while (j < delay)
				j++;
			i++;
		}
	}
}
