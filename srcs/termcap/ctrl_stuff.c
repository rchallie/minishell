/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:55:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/27 19:46:03 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void		clear_screen_(int *key, t_line *line)
{
	(void)key;
	if (line->start.row > 1)
		tputs(tgoto(tgetstr("SF", NULL), 0, line->start.row - 1), 1,
			&tc_putchar);
	line->start.row = 1;
	set_curpos(line);
}

void		delete_char_ctrl_d(t_line *line)
{
	if (line->cursor == line->length)
		return ;
	ft_memmove(line->cmd + line->cursor, line->cmd + line->cursor + 1,
			MAX_CMD_LEN - line->cursor - 1);
	line->length--;
	tputs(tgetstr("cd", NULL), 0, &tc_putchar);
	ft_putstr_fd(line->cmd + line->cursor, 0);
	set_curpos(line);
}

void		exit_pgm(int *key, t_line *line)
{
	char	**exit;
	int		*int_exit;

	(void)key;
	if (ft_strcmp(line->cmd, "") == 0)
	{
		exit = new_double_char_tab_init(1, "exit");
		exit[1] = NULL;
		get_sequence(exit, &int_exit);
		treat_command(exit, int_exit);
	}
	else
		delete_char_ctrl_d(line);
}
