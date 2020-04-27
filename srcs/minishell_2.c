/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:38:48 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/27 11:48:09 by excalibur        ###   ########.fr       */
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

static void		print_middle_term(char *str, int str_len, int row)
{
	t_line		line;

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

void			put_beg(void)
{
	clear_term();
	print_middle_term("┌┬┐\e[92m┬┌┐┌┬┌─┐┬ ┬┌\e[96m─┐\e[91m┬  ┬  ", 24, 1);
	print_middle_term("\e[91m││││││││└─┐\e[95m├─┤├┤ │  \e[92m│  ", 24, 2);
	print_middle_term("\e[91m┴ ┴\e[93m┴┘└┘┴└─┘┴ ┴└─┘┴─\e[92m┘┴─┘", 24, 3);
	print_middle_term("", 0, 4);
	default_term_mode();
	return ;
}

int				has_redir_output(t_minishell *ms, int redir_type,
	int cursor, int fd)
{
	int			o;
	int			s;
	// extern int	errno;

	s = S_IRWXG | S_IRWXU | S_IRWXO;
	if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
		|| ms->sequence[cursor] == 7 || ms->sequence[cursor] == 9)
		return (fd);
	else if (ms->sequence[cursor] == 2)
		return (has_redir_output(ms, redir_type, cursor + 1, fd));
	else if (ms->sequence[cursor] == 4 || ms->sequence[cursor] == 3)
		return (has_redir_output(ms, ms->sequence[cursor], cursor + 1, fd));
	else if (ms->sequence[cursor] == 8 && (redir_type == 4 || redir_type == 3))
	{
		(fd >= 3) ? close(fd) : 0;
		o = (redir_type == 3) ? O_CREAT | O_RDWR | O_TRUNC : O_CREAT | O_RDWR;
		if (!(fd = open(ms->treated[cursor], o, s)))
			return (-1); //Error path à gérer
		redir_type = 0;
	}
	return (has_redir_output(ms, redir_type, cursor + 1, fd));
}

int				has_redir_input(t_minishell *ms, int redir_type,
	int cursor, int fd)
{
	if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
		|| ms->sequence[cursor] == 7 || ms->sequence[cursor] == 9)
		return (fd);
	else if (ms->sequence[cursor] == 2)
		return (has_redir_input(ms, redir_type, cursor + 1, fd));
	else if (ms->sequence[cursor] == 5)
		return (has_redir_input(ms, ms->sequence[cursor], cursor + 1, fd));
	else if (ms->sequence[cursor] == 8 && redir_type == 5)
	{
		(fd >= 3) ? close(fd) : 0;
		if (!(fd = open(ms->treated[cursor], O_RDONLY)))
			return (-1); //Error path à gérer
		redir_type = 0;
	}
	return (has_redir_input(ms, redir_type, cursor + 1, fd));
}
