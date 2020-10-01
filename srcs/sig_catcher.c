/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_catcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 18:19:10 by excalibur         #+#    #+#             */
/*   Updated: 2020/10/01 20:05:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void		sigcatcher_init(void)
{
	if (signal(SIGINT, sig_catcher_minishell) == SIG_ERR)
		exit(ERROR_SIGINT);
	if (signal(SIGQUIT, sig_catcher_minishell) == SIG_ERR)
		exit(ERROR_SIGQUIT);
}

void		sig_catcher_minishell(int num)
{
	if (num == SIGINT)
	{
		write(2, "\n", 1);
		g_ms.last_cmd_rtn = 130;
		(g_ms.entry) ? free(g_ms.entry) : 0;
		g_ms.entry = NULL;
		if (print_prompt() == ERROR)
			exit(1);
	}
	else if (num == SIGQUIT)
		ft_printf(2, "\b\b  \b\b");
}

void		sig_catcher_exec(int num)
{
	if (num == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", 19);
		g_ms.last_cmd_rtn = 131;
	}
	else if (num == SIGINT)
	{
		g_ms.last_cmd_rtn = 130;
		ft_printf(2, "\n");
	}
}
