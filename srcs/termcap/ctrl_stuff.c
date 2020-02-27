/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:55:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 12:55:46 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	clear_screen_(t_line *line)
{
	tputs(tgoto(tgetstr("SF", NULL), 0, line->start.row - 1), 1, &tc_putchar);
	line->start.row = 1;
	set_curpos(line);
}

void	exit_pgm(t_line *line)
{
	ft_printf("ctrl-D");
}
