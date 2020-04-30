/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/30 17:10:38 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		exit_minishell(int argc, char **argv, char **env)
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
