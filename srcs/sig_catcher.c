/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_catcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 18:19:10 by excalibur         #+#    #+#             */
/*   Updated: 2020/08/11 11:34:26 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

void		sigcatcher_init(void)
{
	if (signal(SIGINT, sigint_catcher) == SIG_ERR)
		exit(ERROR_SIGINT);
	if (signal(SIGQUIT, sigquit_catcher) == SIG_ERR)
		exit(ERROR_SIGQUIT);
}

void		sigint_catcher(void)
{
	write(1, "\n", 1);
	g_ms.last_cmd_rtn = 130;
}

void		sigquit_catcher(void)
{
	write(1, "Quit (core dumped)\n", 19);
	g_ms.last_cmd_rtn = 131;
}
