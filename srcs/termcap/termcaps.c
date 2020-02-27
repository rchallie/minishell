/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:00:41 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 17:52:11 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

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
    // ft_printf("\e[44m\e[45m");
	// ft_printf("str[0] = %x, str[1] = %x, str[2] = %x, str[3] = %x, str[4] = %x, str[5] = %x, str[6] = %x\n", (int)str[0], (int)str[1], (int)str[2], (int)str[3], (int)str[4], (int)str[5], (int)str[6]);
    // ft_printf("\e[49m");
    key = match_key_curse(str);
    free(str);
    return (key);
}

void    input_loop(t_minishell *ms, t_line *line)
{
    int     key;
    int save;
    
    save = line->cursor;
    while (42)
    {
        key = get_key();
        ft_getwinsz(&line->winsz);
		match_move(key, line);
        match_hist(key, line);
		match_ctrl(ms, key, line);
        if (key > 31 && key < 127)
        {
            insert_char(line, key);
            save = line->cursor;
            line->cursor = 0;
            set_curpos(line);
            tputs(tgetstr("cd", NULL), 0, &tc_putchar);
	        ft_putstr_fd(line->cmd + line->cursor, 0);
            line->cursor = save;
            if (line->start.row + ((line->length + line->start.col - 1) / line->winsz.col) > line->winsz.row)
		    {
                tputs(tgoto(tgetstr("sf", NULL), 0, line->start.row - 1), 1, &tc_putchar);
		    	line->start.row--;
                set_curpos(line);
		    }
        }
        if (key == 127)
            delete_char(line, key);
        // other if [ctrl-rdm]
		// ctrl-D exit(0)
		// ctrl-C line.cmd = NULL
        set_curpos(line);
        if ((char)key == '\n')
			break ;
	}
    return ;
}

char	*edit_line(t_minishell *ms)
{
    t_line  line;

    // write(1, "begin_loop\n", 11);
    raw_term_mode();
    ft_bzero(&line, sizeof(line));
	ft_bzero(&line.cmd, sizeof(char) * 4096);
    get_cursor_start_pos(&line);
    // write(1, "bef_geth\n", 9);
    line.hist = get_history();
    // write(1, "af_geth\n", 8);
    line.hist_size = ft_dlst_size(line.hist);
    input_loop(ms, &line);
    // write(1, "out_loop\n", 9);
    append_history(line.cmd);
    // write(1, "append_ok\n", 10);
    // if (line.hist)
    //     ft_dlst_del(&line.hist);
    // write(1, "after_del\n", 10);
    default_term_mode();
	if (line.cmd[0] == '\0')
		return (ft_strdup(""));
    return (ft_strdup((char *)line.cmd));
}

int		line_edition(t_minishell *ms)
{
	char	*new_entry;

	new_entry = NULL;
    default_term_mode();	
    init_terminal_data();	
    // interrogate_terminal();
	new_entry = edit_line(ms);
	ms->entry = new_entry;
	return (1);
}
