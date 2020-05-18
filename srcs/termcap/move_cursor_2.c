/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:43:12 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/15 15:36:14 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	right_word(t_line *line)
{
	while (is_not_spec(line->cmd[line->cursor]) && line->cursor != line->length)
		cursor_to_right(line);
	while (is_spec(line->cmd[line->cursor]) && line->cursor != line->length)
		cursor_to_right(line);
	reset_line(line);
}

void	cursor_to_home(t_line *line)
{
	line->cursor = 0;
	set_curpos(line);
	reset_line(line);
}

void	cursor_to_end(t_line *line)
{
	line->cursor = line->length;
	set_curpos(line);
	reset_line(line);
}

void	up_row(t_line *line)
{
	if (line->cursor >= line->winsz.col)
	{
		line->cursor -= line->winsz.col;
		set_curpos(line);
	}
	else
		cursor_to_home(line);
	reset_line(line);
}

void	down_row(t_line *line)
{
	if (line->length > line->winsz.col
		&& line->cursor <= (line->length - line->winsz.col))
	{
		ft_printf(STDIN_FILENO, "nn\n");
		line->cursor += line->winsz.col;
		set_curpos(line);
	}
	else
		cursor_to_end(line);
	reset_line(line);
}
