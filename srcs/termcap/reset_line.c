/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 01:23:02 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/28 01:23:04 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void		reset_line(t_line *line)
{
	int		save;

	save = line->cursor;
	line->cursor = 0;
	set_curpos(line);
	tputs(tgetstr("cd", NULL), 0, &tc_putchar);
	ft_putstr_fd(line->cmd, 0);
	line->cursor = save;
	set_curpos(line);
	return ;
}

void		set_line(int save, t_line *line)
{
	save = line->cursor;
	line->cursor = 0;
	set_curpos(line);
	tputs(tgetstr("cd", NULL), 0, &tc_putchar);
	ft_putstr_fd(line->cmd + line->cursor, 0);
	line->cursor = save;
	if (line->start.row + ((line->length + line->start.col - 1)
		/ line->winsz.col) > line->winsz.row)
	{
		tputs(tgoto(tgetstr("sf", NULL), 0,
			line->start.row - 1), 1, &tc_putchar);
		line->start.row--;
		set_curpos(line);
	}
	return ;
}
