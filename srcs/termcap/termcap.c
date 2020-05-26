/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 21:10:30 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/19 19:02:15 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int			get_key(void)
{
	char	*str;
	int		key;

	key = 0;
	str = NULL;
	if (!(str = malloc(sizeof(char) * (MAX_KEY_LEN + 1))))
		return (-1);
	ft_bzero(str, (sizeof(char) * MAX_KEY_LEN));
	read(0, str, MAX_KEY_LEN);
	str[6] = '\0';
	key = match_key_curse(str);
	free(str);
	return (key);
}

static int	ctrl_c(t_line *line)
{
	g_ms.last_cmd_rtn = 130;
	insert_char(line, '^');
	insert_char(line, 'C');
	return (ERROR);
}

int			input_loop(t_line *line)
{
	int		key;
	int		save;

	save = line->cursor;
	while (42)
	{
		key = get_key();
		if (key != KEY_SLEFT && key != KEY_SRIGHT)
			line->cursor_highl = -1;
		ft_getwinsz(&line->winsz);
		find_match(&key, line);
		if (key > 31 && key < 127)
		{
			insert_char(line, key);
			set_line(save, line);
		}
		else if (key == 127)
			delete_char(line, key);
		set_curpos(line);
		if (key == 3)
			return (ctrl_c(line));
		else if ((char)key == '\n')
			break ;
	}
	return (SUCCESS);
}

char		*edit_line(void)
{
	t_line	line;
	int		input_rtn;

	raw_term_mode();
	ft_bzero(&line, sizeof(line));
	ft_bzero(&line.cmd, sizeof(char) * 4096);
	line.cursor_highl = -1;
	get_cursor_start_pos(&line);
	input_rtn = input_loop(&line);
	cursor_to_end(&line);
	default_term_mode();
	ft_putchar('\n');
	if (input_rtn == ERROR)
		return (ft_strdup(""));
	return (ft_strdup((char *)line.cmd));
}

void		line_edition(void)
{
	default_term_mode();
	init_terminal_data();
	g_ms.entry = edit_line();
}
