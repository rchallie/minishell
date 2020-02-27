/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:55:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 17:53:14 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void 	clear_screen_(t_minishell *ms, t_line *line)
{
	(void)ms;
	tputs(tgoto(tgetstr("SF", NULL), 0, line->start.row - 1), 1, &tc_putchar);
	line->start.row = 1;
	set_curpos(line);
}

void	exit_pgm(t_minishell *ms, t_line *line)
{
	int a = line->length;
	a++;
	exit_minishell(ms);
}
