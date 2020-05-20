/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/20 18:02:50 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: exit_minishell
**	--------------------
**		Exit command. Leave minishell propely.
**
**		(char *)	path : final path to directory.
**		(int)		curosr : arguments cursor.
**		(char **)	argv : arguments.
*/

int		exit_minishell(
	int argc,
	char **argv,
	char **env
)
{
	(void)argc;
	(void)argv;
	(void)env;
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);
	if (g_envp)
		free_double_char_tab(g_envp);
	if (g_export_vars)
		free_double_char_tab(g_export_vars);
	exit(0);
}
