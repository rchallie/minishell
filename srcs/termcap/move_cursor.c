/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:42:56 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 12:43:02 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	cursor_to_left(t_line *line)
{
	if (!line->cursor)
	{
		reset_line(line);
		return ;
	}
	line->cursor--;
	set_curpos(line);
	reset_line(line);
}

void	cursor_to_right(t_line *line)
{
	if (line->cursor == line->length)
	{
		reset_line(line);
		return ;
	}
	line->cursor++;
	set_curpos(line);
	reset_line(line);
}

int		is_spec(char c)
{
	return (c == ' ' || c == '\'' || c == '\"' || c == ';' || c == '|'
		|| c == '>' || c == '<');
}

int		is_not_spec(char c)
{
	return (c != ' ' && c != '\"' && c != '\'' && c != '|' && c != ';'
		&& c != '<' && c != '>');
}

void	left_word(t_line *line)
{
	int		save;

	save = line->cursor;
	if (is_spec(line->cmd[line->cursor - 1]))
		cursor_to_left(line);
	while (is_spec(line->cmd[line->cursor]) && line->cursor)
		cursor_to_left(line);
	while (is_not_spec(line->cmd[line->cursor]) && line->cursor)
		cursor_to_left(line);
	if (line->cursor || (is_spec(line->cmd[0]) && is_not_spec(line->cmd[1])
		&& save != 1 && save != 0))
		cursor_to_right(line);
	reset_line(line);
}
