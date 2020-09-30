/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 18:35:30 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/30 16:32:40 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void			clear_term(void)
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

static void		print_middle_term(char *str1, char *str2, int str_len, int row)
{
	t_line		line;

	raw_term_mode();
	ft_bzero(&line, sizeof(line));
	get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	line.start.row = row;
	line.start.col = line.winsz.col / 2 - str_len / 2;
	set_curpos(&line);
	ft_printf(STDOUT_FILENO, str1);
	ft_printf(STDOUT_FILENO, str2);
	line.start.row = row + 1;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}

void			put_beg(void)
{
	clear_term();
	print_middle_term("\e[91m┌┬┐\e[92m┬┌┐\e[93m┌┬┌\e[94m─┐┬",
		"\e[95m ┬┌\e[96m─┐\e[91m┬  \e[92m┬  ", 24, 1);
	print_middle_term("\e[91m││\e[92m│││\e[93m│││\e[94m└─┐",
		"\e[95m├─┤\e[96m├┤ \e[91m│  \e[92m│  ", 24, 2);
	print_middle_term("\e[91m┴ \e[92m┴\e[93m┴┘└┘\e[94m┴└─",
		"\e[95m┘┴ \e[96m┴└─\e[91m┘┴─\e[92m┘┴─┘", 24, 3);
	print_middle_term("", "", 0, 4);
	default_term_mode();
	return ;
}
