/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_inputs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:48:53 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/02 11:20:34 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int								match_key_curse(char *str)
{
	int							i;
	static struct s_keymatch	key_couple[12] = {
		{KEY_CODE_RI, KEY_RIGHT},
		{KEY_CODE_LE, KEY_LEFT},
		{KEY_CODE_CTRL_LE, KEY_CTRL_LE},
		{KEY_CODE_CTRL_RI, KEY_CTRL_RI},
		{KEY_CODE_CTRL_UP, KEY_CTRL_UP},
		{KEY_CODE_CTRL_DO, KEY_CTRL_DO},
		{KEY_CODE_SHIFT_LE, KEY_SLEFT},
		{KEY_CODE_SHIFT_RI, KEY_SRIGHT},
		{KEY_CODE_CTRL_D, KEY_CTRL_D},
		{KEY_CODE_CTRL_L, KEY_CTRL_L},
		{KEY_CODE_HOME, KEY_HOME},
		{KEY_CODE_END, KEY_END}
	};

	i = 0;
	while (i < 12)
	{
		if (!ft_strcmp(key_couple[i++].key_code, str))
			return (key_couple[i - 1].key_ret);
	}
	return ((int)str[0]);
}

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

void							match_ctrl(int key,
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
			keymove[i - 1].funct(line);
}

void							find_match(int key,
t_line *line)
{
	match_move(key, line);
	match_ctrl(key, line);
}
