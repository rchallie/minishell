/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_catcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 18:19:10 by excalibur         #+#    #+#             */
/*   Updated: 2020/05/19 18:59:31 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

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
