/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:00:41 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/20 16:00:42 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/termcaps.h"

int	match_key_curse(char *str)
{
    int i;
    i = 0;
	static struct s_keymatch	key_couple[4] = {
		{KEY_CODE_UP, KEY_UP},
		{KEY_CODE_DO, KEY_DOWN},
		{KEY_CODE_RI, KEY_RIGHT},
		{KEY_CODE_LE, KEY_LEFT}
	};
	while (i < 4)
		if (!ft_memcmp(key_couple[i++].key_code, str, MAX_KEY_LEN))
			return (key_couple[i - 1].key_ret);
	return (str[0]);
}

int     get_key(void)
{
    char    *str;
    int     key;
    
    key = 0;
    if (!(str = malloc(sizeof(char) * MAX_KEY_LEN)))
        return (-1);
    read(0, str, 1);
    str[1] = '\0';
    if (str[0] == '\x1b')
        read(0, str + 1, MAX_KEY_LEN - 1);
    key = match_key_curse(str);
    free(str);
    return (key);
}

int		tc_putc(int c)
{
	return (write(0, &c, 1));
}

void	set_curpos(t_line *line)
{
	size_t	xpos;
	size_t	ypos;

	xpos = line->start.col + line->cursor % line->winsz.col;
	ypos = line->start.row + line->cursor / line->winsz.col;
	if (xpos > line->winsz.col)
	{
		xpos %= line->winsz.col;
		ypos++;
	}
	tputs(tgoto(tgetstr("cm", NULL), xpos - 1, ypos - 1), 0, &tc_putc);
}

void	cursor_to_left(t_line *line)
{
	if (!line->cursor)
		return ;
	line->cursor--;
    set_curpos(line);
}

void	cursor_to_right(t_line *line)
{
	if (line->cursor == line->length)
		return ;
	line->cursor++;
	set_curpos(line);
}

void	match_move(int key, t_line *line)
{
	int						i;
	static struct s_keymove	keymove[2] = {
		{KEY_RIGHT, &cursor_to_right},
		{KEY_LEFT, &cursor_to_left}
	};

	i = 0;
	while (i < 2)
        if (key == keymove[i++].key)
        	keymove[i - 1].p(line);
}

int		ft_getwinsz(t_winsz *winsz)
{
	struct winsize	w;

	if (ioctl(0, TIOCGWINSZ, &w))
		return (1);
	winsz->col = w.ws_col;
	winsz->row = w.ws_row;
	return (0);
}

void	insert_char(t_line *line, int key)
{
	if (line->length + 1 >= MAX_CMD_LEN)
		return ;
	ft_memmove(line->cmd + line->cursor + 1, line->cmd + line->cursor,
			MAX_CMD_LEN - line->cursor - 1);
	line->cmd[line->cursor] = key;
	line->length++;
	line->cursor++;
	tputs(tgetstr("im", NULL), 1, &tc_putc);
	ft_putchar_fd(key, 0);
	tputs(tgetstr("ei", NULL), 1, &tc_putc);
}

void	delete_char(t_line *line, int key)
{
	if (key == 127 && !line->cursor)
		return ;
	if (key == 127)
		cursor_to_left(line);
	if (line->cursor == line->length)
		return ;
	ft_memmove(line->cmd + line->cursor, line->cmd + line->cursor + 1,
			MAX_CMD_LEN - line->cursor - 1);
	line->length--;
	tputs(tgetstr("cd", NULL), 0, &tc_putc);
	ft_putstr_fd(line->cmd + line->cursor, 0);
	set_curpos(line);
}

void    input_loop(t_line *line)
{
    int     key;

    while (1)
    {
        while (1)
            if ((key = get_key()) != 0)
                break ;
        ft_getwinsz(&line->winsz);
        if (line->start.row + line->cursor / line->winsz.col > line->winsz.row)
			line->start.row--;
        match_move(key, line);
        // match_move_or_hist (up, left, down, right, left_word, right_word, etc)
        if (key > 31 && key < 127)
            insert_char(line, key);
        if (key == 127)
            delete_char(line, key);
        // other if [ctrl-rdm]
        if ((char)key == '\n')
            break ;
    }
}

void	get_cursor_start_pos(t_line *line)
{
	char	answer[20];
	int		i;

	ft_putstr_fd("\e[6n", 0);
	ft_bzero(answer, 20);
	i = read(0, answer, 20);
	answer[i] = 0;
	i = 2;
	line->start.row = ft_atoi(answer + i);
	while (ft_isdigit(answer[i]))
		i++;
	line->start.col = ft_atoi(answer + i + 1);
    return ;
}

char    *edit_line(void)
{
    t_line  line;

    raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
    input_loop(&line);
    // put_cursor_to_end + add \n + append_history + delstr
    default_term_mode();
    return (ft_strdup((char *)line.cmd));
}

int     line_edition(void)
{
    char    *str;

    default_term_mode();
    init_terminal_data();
    interrogate_terminal();
    str = edit_line();
	default_term_mode();
    return (0);
}
