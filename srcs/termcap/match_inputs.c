/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_inputs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 12:48:53 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 17:57:07 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int								match_key_curse(char *str)
{
	int							i;
	static struct s_keymatch	key_couple[14] = {
		{KEY_CODE_UP, KEY_UP},
		{KEY_CODE_DO, KEY_DOWN},
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
	while (i < 14)
	{
		if (!ft_strcmp(key_couple[i++].key_code, str))
			return (key_couple[i - 1].key_ret);
	}
	return ((int)str[0]);
}

void	find_match(t_minishell *ms, int key, t_line *line)
{
	match_move(key, line);
	match_highlight(key, line);
	match_ctrl(ms, key, line);
	match_hist(key, line);
}