/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:55:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/18 15:58:29 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	clear_screen_(int *key, t_line *line)
{
  if (line->start.row > 1)
		tputs(tgoto(tgetstr("SF", NULL), 0, line->start.row - 1), 1, &tc_putchar);
	line->start.row = 1;
	set_curpos(line);
}

void	exit_pgm(int *key, t_line *line)
{
	int		i;
	char	*str;

	i = -1;
	str = "exit";
	if (line->length != 0)
		return;
	while (++i != 5)
		line->cmd[i] = str[i];
	*key = '\n';
}
