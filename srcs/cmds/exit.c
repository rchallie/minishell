/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 16:16:06 by excalibur        ###   ########.fr       */
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
	default_term_mode();
	free_double_char_tab(ms.treated);
	free(ms.sequence);
	if (envp)
		free_double_char_tab(envp);
	if (export_vars)
		free_double_char_tab(export_vars);
	exit(0);
}
