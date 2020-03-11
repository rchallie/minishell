/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:38:48 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/10 18:48:26 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void		clear_term(void)
{
	t_line line;

	raw_term_mode();
	ft_bzero(&line, sizeof(line));
	get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
	line.start.row = 0;
	line.start.col = 0;
	line.cursor = ft_strlen("[minishell] /minishell :");
	set_curpos(&line);
	default_term_mode();
}


static void	print_middle_term(char *str, int str_len, int row)
{
	t_line line;

	raw_term_mode();
	ft_bzero(&line, sizeof(line));
	get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	line.start.row = row;
	line.start.col = line.winsz.col / 2 - str_len / 2;
	set_curpos(&line);
	ft_printf(str);
	line.start.row = row + 1;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}

void		put_beg(void)
{
	clear_term();
	print_middle_term("\e[91m┌┬┐\e[92m┬┌┐\e[93m┌┬┌\e[94m─┐┬\e[95m ┬┌\e[96m─┐\e[91m┬  \e[92m┬  ", 24,1);
	print_middle_term("\e[91m││\e[92m│││\e[93m│││\e[94m└─┐\e[95m├─┤\e[96m├┤ \e[91m│  \e[92m│  ", 24,2);
	print_middle_term("\e[91m┴ \e[92m┴\e[93m┴┘└┘\e[94m┴└─\e[95m┘┴ \e[96m┴└─\e[91m┘┴─\e[92m┘┴─┘", 24,3);
	print_middle_term("", 0, 4);
	default_term_mode();
	return ;
}
