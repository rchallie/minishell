/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_inputs_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 03:16:27 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/28 03:17:01 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void							match_move(int key, t_line *line)
{
	int							i;
	static struct s_keymove		keymove[8] = {
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

void							match_highlight(int key, t_line *line)
{
	int							i;
	static struct s_keymove_hl	keymove_hl[2] = {
		{KEY_SLEFT, &highlight},
		{KEY_SRIGHT, &highlight}
	};

	i = 0;
	while (i < 2)
		if (key == keymove_hl[i++].key)
			keymove_hl[i - 1].funct(key, line);
}


void							match_ctrl(t_minishell *ms, int key,
t_line *line)
{
	int							i;
	static struct s_keymove_ms	keymove[2] = {
		{KEY_CTRL_L, &clear_screen_},
		{KEY_CTRL_D, &exit_pgm}
	};

	i = 0;
	while (i < 2)
		if (key == keymove[i++].key)
			keymove[i - 1].funct(ms, line);
}

void							match_hist(int key, t_line *line)
{
	int							i;
	static struct s_keyhist		keyhist[2] = {
		{KEY_UP, &old_history},
		{KEY_DOWN, &new_history}
	};

	i = 0;
	while (i < 2)
		if (key == keyhist[i++].key)
			keyhist[i - 1].funct(line, (&line->hist));
}
