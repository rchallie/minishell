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

#include "../../incs/minishell.h"

int	match_key_curse(char *str)
{
    int i;
    i = 0;
	static struct s_keymatch	key_couple[12] = {
		{KEY_CODE_UP, KEY_UP},
		{KEY_CODE_DO, KEY_DOWN},
		{KEY_CODE_RI, KEY_RIGHT},
		{KEY_CODE_LE, KEY_LEFT},
		{KEY_CODE_CTRL_LE, KEY_CTRL_LE},
		{KEY_CODE_CTRL_RI, KEY_CTRL_RI},
		{KEY_CODE_CTRL_UP, KEY_CTRL_UP},
		{KEY_CODE_CTRL_DO, KEY_CTRL_DO},
		{KEY_CODE_CTRL_D, KEY_CTRL_D},
		{KEY_CODE_CTRL_L, KEY_CTRL_L},
		{KEY_CODE_HOME, KEY_HOME},
		{KEY_CODE_END, KEY_END}
	};
	while (i < 12)
	{
		if (!ft_strcmp(key_couple[i].key_code, str))
			return (key_couple[i].key_ret);
		i++;
	}
	return ((int)str[0]);
}

int     get_key(void)
{
    char    *str;
    int     key;
    
    key = 0;
	str = NULL;
    if (!(str = malloc(sizeof(char) * (MAX_KEY_LEN + 1))))
        return (-1);
	ft_bzero(str, (sizeof(char) * MAX_KEY_LEN));
	read(0, str, MAX_KEY_LEN);
	str[6] = '\0';
	// printf("str[0] = %d, str[1] = %d, str[2] = %d, str[3] = %d, str[4] = %d, str[5] = %d, str[6] = %d\n", (int)str[0], (int)str[1], (int)str[2], (int)str[3], (int)str[4], (int)str[5], (int)str[6]);
    // printf("strcmp(str, KEY_CODE_CTRL_LE = %d\n", ft_strcmp(str, KEY_CODE_CTRL_LE));
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

int		is_spec(char c)
{
	return (c == ' ' || c == '\'' || c == '\"' || c == ';' || c == '|'
		|| c == '>' || c == '<');
}

int		is_not_spec(char c)
{
	return (c != ' ' && c != '\"' && c != '\'' && c != '|' && c != ';'
		&& c != '<' && c != '>');
}

void	left_word(t_line *line)
{
	int		save;

	save = line->cursor;
	if (is_spec(line->cmd[line->cursor - 1]))
		cursor_to_left(line);
	while (is_spec(line->cmd[line->cursor]) && line->cursor)
		cursor_to_left(line);
	while (is_not_spec(line->cmd[line->cursor]) && line->cursor)
		cursor_to_left(line);
	if (line->cursor || (is_spec(line->cmd[0]) && is_not_spec(line->cmd[1])
		&& save != 1 && save != 0))
		cursor_to_right(line);
}


void	right_word(t_line *line)
{
	while (is_not_spec(line->cmd[line->cursor]) && line->cursor != line->length)
		cursor_to_right(line);
	while (is_spec(line->cmd[line->cursor]) && line->cursor != line->length)
		cursor_to_right(line);
}

void	cursor_to_home(t_line *line)
{
	line->cursor = 0;
	set_curpos(line);
}

void	cursor_to_end(t_line *line)
{
	line->cursor = line->length;
	set_curpos(line);
}

void	up_row(t_line *line)
{
	if (line->cursor >= line->winsz.col)
	{
		line->cursor -= line->winsz.col;
		set_curpos(line);
	}
	else
		cursor_to_home(line);
}

void	down_row(t_line *line)
{
	if (line->cursor <= (line->length - line->winsz.col))
	{
		line->cursor += line->winsz.col;
		set_curpos(line);
	}
	else
		cursor_to_end(line);
}

void	match_move(int key, t_line *line)
{
	int						i;
	static struct s_keymove	keymove[8] = {
		{KEY_RIGHT, &cursor_to_right},
		{KEY_LEFT, &cursor_to_left},
		{KEY_CTRL_LE, &left_word},
		{KEY_CTRL_RI, &right_word},
		{KEY_CTRL_UP, &up_row},
		{KEY_CTRL_DO, &down_row},
		{KEY_HOME, &cursor_to_home},
		{KEY_END, &cursor_to_end}
	};

	i = 0;
	while (i < 8)
        if (key == keymove[i++].key)
        	keymove[i - 1].funct(line);
}

void 	clear_screen_(t_line *line)
{
	char *str;
	char	*pwd;

	if (!get_pwd_short(&pwd))
			return ;
	str = tgetstr("cl", NULL);
	ft_putstr_fd(str, 0);
	ft_printf("[minishell] %s > %s", pwd, line->cmd);
	line->start.row = 0;
	set_curpos(line);
} 	

void	exit_pgm(t_line *line)
{
	int a = line->length;
	a++;
	printf("ctrl-D");
}

void	match_ctrl(int key, t_line *line)
{
	int						i;
	static struct s_keymove	keymove[2] = {
		{KEY_CTRL_L, &clear_screen_},
		{KEY_CTRL_D, &exit_pgm}
	};

	i = 0;
	while (i < 2)
        if (key == keymove[i++].key)
        	keymove[i - 1].funct(line);
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
	tputs(tgetstr("cd", NULL), 0, &tc_putc);
	ft_putstr_fd(line->cmd + line->cursor, 0);
	tputs(tgetstr("ei", NULL), 1, &tc_putc);
	set_curpos(line);
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
		key = get_key();
        ft_getwinsz(&line->winsz);
        if (line->start.row + ((line->length + line->start.col) / line->winsz.col) > line->winsz.row)
		{
			tputs(tgetstr("sf", NULL), 0, &tc_putc);
			line->start.row--;
		}
		match_move(key, line);
		match_ctrl(key, line);
        // match_hist (up, down etc)
        if (key > 31 && key < 127)
            insert_char(line, key);
        if (key == 127)
            delete_char(line, key);
        // other if [ctrl-rdm]
		// ctrl-D exit(0)
		// ctrl-C line.cmd = NULL
        if ((char)key == '\n')
        {
			line->cmd[line->length] = '\n';
			line->cmd[line->length + 1] = '\0';
			break ;
		}
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

char	*edit_line(void)
{
    t_line  line;

    raw_term_mode();
    ft_bzero(&line, sizeof(line));
	ft_bzero(&line.cmd, sizeof(char) * 4096);
    get_cursor_start_pos(&line);
    input_loop(&line);
    // append_history + delstr
    default_term_mode();
	if (line.cmd[0] == '\0')
		return (ft_strdup(""));
    return (ft_strdup((char *)line.cmd));
}

int		line_edition(char **entry)
{
	char	*new_entry;

	new_entry = NULL;
    default_term_mode();	
    init_terminal_data();	
    // interrogate_terminal();
	new_entry = edit_line();
	*entry = new_entry;
	return (0);
}
