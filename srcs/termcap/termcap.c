/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 21:10:30 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/28 09:52:25 by rchallie         ###   ########.fr       */
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

void		input_loop(t_minishell *ms, t_line *line)
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
		find_match(ms, key, line);
		if (key > 31 && key < 127)
		{
			insert_char(line, key);
			set_line(save, line);
		}
		if (key == 127)
			delete_char(line, key);
		set_curpos(line);
		if ((char)key == '\n')
			break ;
	}
	return ;
}

char		*edit_line(t_minishell *ms)
{
	t_line	line;

	raw_term_mode();
	ft_bzero(&line, sizeof(line));
	ft_bzero(&line.cmd, sizeof(char) * 4096);
	line.cursor_highl = -1;
	get_cursor_start_pos(&line);
	input_loop(ms, &line);
	cursor_to_end(&line);
	default_term_mode();
	ft_putchar('\n');
	return (ft_strdup((char *)line.cmd));
}

int			line_edition(t_minishell *ms)
{
	char	*new_entry;

	new_entry = NULL;
	default_term_mode();
	init_terminal_data();
	new_entry = edit_line(ms);
	ms->entry = new_entry;
	return (1);
}
