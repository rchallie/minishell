/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:55:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/19 18:54:22 by excalibur        ###   ########.fr       */
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

void		exit_pgm(int *key, t_line *line)
{
	char	**exit;
	int		*int_exit;

	(void)key;
	(void)line;
	exit = new_double_char_tab_init(1, "exit");
	get_sequence(exit, &int_exit);
	treat_command(exit, int_exit);
}
